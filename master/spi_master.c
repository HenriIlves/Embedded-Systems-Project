#include "spi_master.h"

#include <avr/io.h>
#include <stdint.h>

void spi_master_init(void)
{
    // SS, MOSI & SCK as outputs
    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);

    SPCR |= (1 << SPE);  // enable SPI
    SPCR |= (1 << MSTR); // select master mode

    SPCR |= (1 << SPR0); // clock rate F_CPU / 16
    SPCR &= ~(1 << SPR1);
}

void spi_master_send_state(state_t state)
{
    PORTB &= ~(1 << PB0); // enable slave

    // clear out status register
    uint8_t trash = SPSR;
    trash = SPDR;

    SPDR = (uint8_t)state; // send byte to slave

    while (!(SPSR & (1 << SPIF))) // wait until transmission over
    {
        continue;
    }

    PORTB |= (1 << PB0); // disable slave
}
