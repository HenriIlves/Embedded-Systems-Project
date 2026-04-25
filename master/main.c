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

//Switch case cases
typedef enum {
	IDLE,
	GOING_UP,
	GOING_DOWN,
	DOOR_OPENING,
	OBSTACLE,
	DOOR_CLOSING,
	FAULT
} State;

int main(void) {
	spi_master_init();

	// Intitialize LCD
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	
	while (1) {
		switch(state) {

			//IDLE
			case IDLE:
                lcd_clear();
                lcd_puts("Choose floor:");
                
                
                //Check for fault
                if(target_floor == current_floor) {
                    state = FAULT;
                }
                break;

			//GOING UP
			case GOING_UP:
                lcd_clear();
                lcd_puts("Current floor: %02d", current_floor);
                break;

			//GOING DOWN
			case GOING_DOWN:
                lcd_clear();
                lcd_puts("Current floor: %02d", current_floor);
                break;

			//DOOR OPENING
			case DOOR_OPENING:
                lcd_clear();
                lcd_puts("Door Open");
                break;

			//OBSTACLE
			case OBSTACLE:
                lcd_clear();
                lcd_puts("Obstacle!");
                break;
			
			//DOOR CLOSING
			case DOOR_CLOSING:
                lcd_clear();
                lcd_puts("Door Closing");
                break;

			//FAULT
			case FAULT:
                lcd_clear();
                lcd_puts("Same Floor!");
                
                _delay_ms(2000);

                state = IDLE;
                break;
        }
	}
	return (0);
}