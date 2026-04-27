#include "spi_slave.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

static volatile state_t current_state = IDLE;

ISR(SPI_STC_vect)
{
    current_state = SPDR;
}

void spi_slave_init(void)
{
    // SS, MOSI & SCK as input
    DDRB &= ~(1 << PB2);
    DDRB &= ~(1 << PB3);
    DDRB &= ~(1 << PB5);

    DDRB |= (1 << PB4); // MISO as output
	
	PORTB |= (1 << PB2); // SS pull-up (high by default)

    SPCR |= (1 << SPE);  // enable SPI
    SPCR |= (1 << SPIE); // enable SPI interrupts
    sei();               // enable global interrupts
}

state_t spi_slave_get_state(void)
{
    return current_state;
}
