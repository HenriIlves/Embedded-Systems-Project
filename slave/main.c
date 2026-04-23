#include "mcu.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "bit_ops.h"
#include "timer1.h"
#include "tune.h"
#include "spi_slave.h"

#define SLAVE_ADDRESS 0b1010111 // 0x57

// Melody for the fault sound
note_t smokeInTheWater[] = {
    {G3, QUARTER},
    {AS3, QUARTER},
    {C4, HALF},
    {0, QUARTER},

    {G3, QUARTER},
    {AS3, QUARTER},
    {CS4, QUARTER},
    {C4, HALF},
    {0, QUARTER},

    {G3, QUARTER},
    {AS3, QUARTER},
    {C4, HALF},
    {0, QUARTER},

    {AS3, QUARTER},
    {G3, FULL},
    {0, FULL},
};

int main(void)
{
    spi_slave_init();

    while (1)
    {
        state_t state = spi_slave_get_state();

        continue;
    }

    return (0);
}
