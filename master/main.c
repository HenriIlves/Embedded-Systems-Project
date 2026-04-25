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

// Global variables
volatile state_t state = IDLE;

int8_t requested_floor = -1;
int8_t current_floor = 0;

// Setup to initialize LCD and Keypad
static void setup(void) {
	setup_uart_io();
	lcd_init(LCD_DISP_ON); // Initialize LCD
	lcd_clrscr();
	
	KEYPAD_Init(); // Initialize Keypad

	// Obstacle button init (PA0 / Digital 22)
	util_BitClear(DDRA, PA0);
	util_BitSet(PORTA, PA0);
	
	printf("Setup Ready.\r\n");
	lcd_puts("Choose floor");
}

// Keypad and IDLE logic
static int8_t floor_keypad_choice(void)
{
	static uint32_t memory = 0;
	uint8_t key_signal = KEYPAD_GetKey();
	
	if (key_signal == NO_KEY_PRESSED) {
		return -1;
	}
	printf("Keypad: %c\r\n", key_signal);
	
	// Clear display & memory
	if (key_signal == '*') {
		memory = 0;
		lcd_clrscr();
		lcd_puts("Cleared");
		return -1;
	}
	if (key_signal == '#') {
		if (memory <= MAX_FLOOR && memory >= MIN_FLOOR){
			int8_t result = (int8_t)memory;
			memory = 0;
			return result;
		}
	}
	
	if (key_signal >= '0' && key_signal <= '9') {
		uint8_t key_value = key_signal - '0'; // ASCII value to numeric value
		if (memory < 100) {
			memory *= 10;
			memory += key_value;
		}
	}
	return -1;
}

// Queued floor requests
#define QUEUE_MAX 10

static uint8_t floor_queue[QUEUE_MAX];
static uint8_t queue_head  = 0;
static uint8_t queue_tail  = 0;
static uint8_t queue_count = 0;

static uint8_t queue_add(uint8_t floor) {
	if (queue_count >= QUEUE_MAX) {
		return 0;
	}

	floor_queue[queue_tail] = floor;
	queue_tail = (queue_tail + 1) % QUEUE_MAX;
	queue_count++;

	return 1;
}

static uint8_t queue_remove(uint8_t *floor) {
	if (queue_count == 0) {
		return 0;
	}

	*floor = floor_queue[queue_head];
	queue_head = (queue_head + 1) % QUEUE_MAX;
	queue_count--;

	return 1;
}

// Returns 1 if any key is currently held on the keypad.
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
			case IDLE:
			lcd_clrscr();
			lcd_puts("Choose floor:");
			requested_floor = floor_keypad_choice();

			// ── Queue the floor; dequeue next target ──
			if (requested_floor != -1) {
				queue_add((uint8_t)requested_floor);
			}
			if (queue_remove(&target_floor)) {
				if ((int8_t)target_floor > current_floor)
				state = GOING_UP;
				else if ((int8_t)target_floor < current_floor)
				state = GOING_DOWN;
				else
				state = FAULT;
			}
			break;

			//GOING UP
			case GOING_UP:
			lcd_clrscr();

			if (current_floor < target_floor) {
				current_floor++;

				snprintf(key_str, sizeof(key_str), "Current floor: %02d", current_floor);
				lcd_puts(key_str);

				_delay_ms(800); //Simulated movement
				} else {
				state = DOOR_OPENING;
			}
			break;

			//GOING DOWN
			case GOING_DOWN:
			 lcd_clrscr();
			
			 if (current_floor > target_floor) {
				 current_floor--;

				 snprintf(key_str, sizeof(key_str), "Current floor: %02d", current_floor);
				 lcd_puts(key_str);

				 _delay_ms(800); //Simulated movement
				 } else {
				 state = DOOR_OPENING;
			 }
			 break;

			//DOOR OPENING
			case DOOR_OPENING:
			lcd_clrscr();
			lcd_puts("Door Open");

			// Obstacle detection: button press on PA0
			if (util_IsBitCleared(PINA, PA0)) {
				state = OBSTACLE_DETECTION;
				break;
			}
			break;

			//OBSTACLE
			case OBSTACLE_DETECTION:
			lcd_clrscr();
			lcd_puts("Obstacle!");

			// Melody stops when any keypad key is pressed
			if (KeypadIsPressed()) {
				state = DOOR_CLOSING;
			}
			break;
			
			//DOOR CLOSING
			case DOOR_CLOSING:
			lcd_clrscr();
			lcd_puts("Door Closing");
			break;

			//FAULT
			case FAULT:
			lcd_clrscr();
			lcd_puts("Same Floor!");
			
			_delay_ms(2000);

			state = IDLE;
			break;
		}

		// Send current state to slave after every switch iteration
		spi_master_send_state(state);
	}
	return (0);
}
