#include "mcu.h"

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <inttypes.h>

#include "uart.h"
#include "keypad.h"
#include "lcd.h"
#include "stdutils.h"
#include "spi_master.h"

#define SLAVE_ADDRESS 0b1010111 // 87 as decimal
#define MIN_FLOOR 0
#define MAX_FLOOR 99
#define NO_KEY_PRESSED  (0xFF)

volatile state_t state = IDLE;
int8_t requested_floor = -1;
int8_t current_floor = 0;

// Setup to initialize LCD and Keypad
// Author: Maija Lehtosaari
static void setup(void) {
	setup_uart_io();
	lcd_init(LCD_DISP_ON); // Initialize LCD
	lcd_clrscr();
	
	KEYPAD_Init(); // Initialize Keypad

	// Obstacle button init (PA0 / Digital 22)
	util_BitClear(DDRA, PA0);
	util_BitSet(PORTA, PA0);
	
	printf("Setup Ready.\r\n");

	lcd_puts("Floor: 0");
	lcd_gotoxy(0,1);
	lcd_puts("Choose floor");
}

// Keypad logic that builds floor number, "*" clears display and "#" returns floor number
// Author: Maija Lehtosaari
static int8_t floor_keypad_choice(void)
{
	static uint32_t memory = 0;
	uint8_t key_signal = KEYPAD_GetKey();
	
	// No key pressed, stay in IDLE
	if (key_signal == NO_KEY_PRESSED) {
		return -1;
	}
	printf("Keypad: %c\r\n", key_signal);
	
	// Clear memory and update display to current floor
	if (key_signal == '*') {
		memory = 0;
		lcd_gotoxy(0,1);
		lcd_puts("Cleared           ");

		_delay_ms(2000);
		lcd_clrscr();
		static char floorstring[16];
		snprintf(floorstring, sizeof(floorstring), "Floor: %02d", current_floor);
		lcd_puts(floorstring);
		lcd_gotoxy(0,1);
		lcd_puts("Choose floor");
		return -1;
	}
	// Builds floor by apppending digits to memory, limit under 100
	if (key_signal >= '0' && key_signal <= '9') {
		uint8_t key_value = key_signal - '0'; // ASCII value to numeric value
		if (memory < 100) {
			memory *= 10; // e.g. 1 * 10 + 3 = 13
			memory += key_value;
		}
		// Updates display to current input
		lcd_clrscr();
		static char dis_mem[32];
		snprintf(dis_mem, sizeof(dis_mem), "Key Pressed: %ld", memory);
		lcd_puts(dis_mem);
	}
	// Confirm and return selected floor
	if (key_signal == '#') {
		if (memory <= MAX_FLOOR && memory >= MIN_FLOOR){
			int8_t result = (int8_t)memory; // Floor number stored to result
			memory = 0; // Memory reset for next entries
			return result;
			}	else {
			memory = 0;
			lcd_gotoxy(0,1);
			lcd_puts("Invalid floor");
			return -1;
		}
	}
	return -1;
}


// Returns 1 if any key is currently held on the keypad.
// Author: Maija Lehtosaari
static uint8_t KeypadIsPressed(void)
{
	M_ROW = 0x0F;
	return ((M_COL & 0x0F) != 0x0F);
}


int main(void) {
	static char key_str[32];
	spi_master_init();
	setup();

	uint8_t target_floor = 0;
	while (1) {
		switch(state) {

			//IDLE
			// Author: Maija Lehtosaari
			case IDLE:
			printf("idle\n\r");
			requested_floor = floor_keypad_choice();

			if (requested_floor != -1) {
				target_floor = (uint8_t)requested_floor;
				
				if ((int8_t)target_floor > current_floor)
					state = GOING_UP;
				else if ((int8_t)target_floor < current_floor)
					state = GOING_DOWN;
				else
					state = FAULT;
			}
			break;

			// GOING UP
			// Author: Maija Lehtosaari
			case GOING_UP:
			lcd_clrscr();
			printf("going up\n\r");

			if (current_floor < target_floor) {
				current_floor++;

				snprintf(key_str, sizeof(key_str), "Floor: %02d", current_floor);
				lcd_puts(key_str);
				lcd_gotoxy(0,1);
				lcd_puts("Going up");

				_delay_ms(800); //Simulated movement
				} else {
				state = DOOR_OPENING;
			}
			break;

			// GOING DOWN
			// Author: Maija Lehtosaari
			case GOING_DOWN:
			lcd_clrscr();
			printf("going down\n\r");
			
			if (current_floor > target_floor) {
				current_floor--;

				snprintf(key_str, sizeof(key_str), "Floor: %02d", current_floor);
				lcd_puts(key_str);
				lcd_gotoxy(0,1);
				lcd_puts("Going down");

				_delay_ms(800); //Simulated movement
				} else {
				state = DOOR_OPENING;
			}
			break;

			// DOOR OPENING
			// Author: Juho Koski
			case DOOR_OPENING:
			lcd_clrscr();
			lcd_puts(key_str);
			lcd_gotoxy(0,1);
			lcd_puts("Door Open");
			printf("door opening\n\r");

			// 30 * 100 ms = 3000 ms = 3s.
			for (uint8_t i = 0; i < 30; i++) {

				// Obstacle detection: button press on PA0
				if (util_IsBitCleared(PINA, PA0)) {
					state = OBSTACLE_DETECTION;
					break;
				}
				
				_delay_ms(100);
			}
			// check current state
			if(state == DOOR_OPENING){
				state= DOOR_CLOSING;
			}

			break;

			// OBSTACLE
			// Author: Olli Kirkkopelto
			case OBSTACLE_DETECTION:
			lcd_clrscr();
			lcd_puts(key_str);
			lcd_gotoxy(0,1);
			lcd_puts("Obstacle!");
			printf("obstacle\n\r");

			while (1) {
				// Melody stops when any keypad key is pressed
				if (KeypadIsPressed()) {
					state = DOOR_CLOSING;
					break;
				}
			}
			
			break;
			
			// DOOR CLOSING
			// Author: Juho Koski
			case DOOR_CLOSING:
			lcd_clrscr();
			lcd_puts(key_str);
			lcd_gotoxy(0,1);
			lcd_puts("Door Closing");
			printf("door closing\n\r");

			_delay_ms(2000);
			
			state = IDLE;

			lcd_clrscr();
			lcd_puts(key_str);
			lcd_gotoxy(0,1);
			lcd_puts("Choose floor");
			break;

			// FAULT
			// Author: Olli Kirkkopelto
			case FAULT:
			lcd_clrscr();
			lcd_puts(key_str);
			lcd_gotoxy(0,1);
			lcd_puts("Same Floor!");
			printf("fault\n\r");
			
			_delay_ms(4000);

			lcd_gotoxy(0,1);
			lcd_puts("Choose floor");
			state = IDLE;
			break;
		}

		static state_t last_sent_state = IDLE;
		if (state != last_sent_state) {
			spi_master_send_state(state);
			last_sent_state = state;
		}
	}
	return (0);
}
