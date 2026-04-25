#ifndef SPI_MASTER_H
#define SPI_MASTER_H

// for ATmega2560

typedef enum state
{
    IDLE,
    GOING_UP,
    GOING_DOWN,
    DOOR_OPENING,
    OBSTACLE_DETECTION,
    DOOR_CLOSING,
    FAULT,
} state_t;

void spi_master_init(void);

void spi_master_send_state(state_t state);

#endif