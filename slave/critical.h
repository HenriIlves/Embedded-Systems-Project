#ifndef CRITICAL_H
#define CRITICAL_H
#include <avr/interrupt.h>

extern uint32_t critical_nesting;

void enter_critical(void);
void exit_critical(void);

#endif
