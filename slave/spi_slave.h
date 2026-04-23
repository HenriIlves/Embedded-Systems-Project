#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H

// for ATmega328p

typedef enum state_t
{
    IDLE,
    GOING_UP,
    GOING_DOWN,
    DOOR_OPENING,
    OBSTACE_DETECTION,
    DOOR_CLOSING,
    FAULT,
} state_t;

void spi_slave_init(void);

state_t spi_slave_get_state(void);

#endif