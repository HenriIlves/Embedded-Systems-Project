#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "uart.h"
#include <inttypes.h>
#include "keypad.h"
#include "lcd.h"
#include "stdutils.h"

#define CMD_IDLE         0x00
#define CMD_GOING_UP     0x01
#define CMD_GOING_DOWN   0x02
#define CMD_DOOR_OPENING 0x03
#define CMD_DOOR_CLOSING 0x04
#define CMD_OBSTACLE     0x05
#define CMD_FAULT        0x06

#define SLAVE_ADDRESS 0b1010111 // 87 as decimal




int main(void) {

    return 0;
}
