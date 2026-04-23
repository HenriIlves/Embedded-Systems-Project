#include "spi_slave.h"

#include <avr/io.h>
#include <stdint.h>

void spi_slave_init(void)
{
    DDRB |= (1 << PB4); // MISO as output
    SPCR |= (1 << SPE); // enable SPI
}

state_t spi_slave_get_state(void)
{
    SPDR = 0; // zero out data returned to master

    while (!(SPSR & (1 << SPIF))) // wait for transfer to end
    {
        continue;
    }

    return (state_t)SPDR; // receive state byte from master
}
