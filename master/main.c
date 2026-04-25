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

int main(void) {
	static char key_str[32];
	spi_master_init();
	setup();
	
	while (1) {
		switch(state) {

			//IDLE
			case IDLE:
			lcd_clrscr();
			lcd_puts("Choose floor:");
			requested_floor = floor_keypad_choice();
			
			if (requested_floor != -1) {
				if (requested_floor > current_floor)
					state = GOING_UP;
				else if (requested_floor < current_floor)
					state = GOING_DOWN; 
				else
					state = FAULT;
			}
			break;

			//GOING UP
			case GOING_UP:
			current_floor++; // placeholder for logic, now just goes up one
			lcd_clrscr();
			snprintf(key_str, sizeof(key_str), "Current floor: %02d", current_floor); // Convert numeric value to string
			break;

			//GOING DOWN
			case GOING_DOWN:
			current_floor--; // placeholder for logic, now just goes down one
			lcd_clrscr();
			snprintf(key_str, sizeof(key_str), "Current floor: %02d", current_floor); // Convert numeric value to string
			break;

			//DOOR OPENING
			case DOOR_OPENING:
			lcd_clrscr();
			lcd_puts("Door Open");
			break;

			//OBSTACLE
			case OBSTACLE:
			lcd_clrscr();
			lcd_puts("Obstacle!");
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
	}
	return (0);
}