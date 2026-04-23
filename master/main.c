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

int main(void)
{
    spi_master_init();

    while (1)
    {
        continue;
    }

    return (0);
}
