#ifndef TIMER0_H
#define TIMER0_H
#include <stdint.h>

void setup_timer0(void);
uint32_t get_time(void);

// Called from Timer0 ISR each tick (every 10 ms). Defined in main.c.
void nonStop_tick(uint32_t ticks);

#endif
