#include "critical.h"
#include <avr/interrupt.h>

uint32_t critical_nesting = 0;

void enter_critical(void) {
    cli();
    critical_nesting++;
}

void exit_critical(void) {
    if (critical_nesting == 0) return;
    critical_nesting--;
    if (critical_nesting == 0) {
        sei();
    }
}
