#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bit_ops.h"
#include "timer1.h"
#include "tune.h"

#define CMD_IDLE         0x00
#define CMD_GOING_UP     0x01
#define CMD_GOING_DOWN   0x02
#define CMD_DOOR_OPENING 0x03
#define CMD_DOOR_CLOSING 0x04
#define CMD_OBSTACLE     0x05
#define CMD_FAULT        0x06

#define SLAVE_ADDRESS    0b1010111   // 0x57

#define LED_MOVEMENT  PD2   // going up / down
#define LED_DOOR_OPEN PD3   // door opening
#define LED_DOOR_CLOSE PD4  // door closing
#define LED_OBSTACLE  PD5   // obstacle


int main(void) {

    return 0;
}
