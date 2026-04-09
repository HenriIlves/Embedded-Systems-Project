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

#define SLAVE_ADDRESS 0b1010111 // 87 as decimal

static void setup(void) {

    return;
}

int main(void) {
    setup();

    return(0);
}

