#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bit_ops.h"
#include "timer1.h"
#include "tune.h"

#define SLAVE_ADDRESS 0b1010111   // 0x57

// Melody for the fault sound
note_t smokeInTheWater[] = {
    { G3, QUARTER },
    { AS3, QUARTER },
    { C4, HALF },
    { 0, QUARTER },

    { G3, QUARTER },
    { AS3, QUARTER },
    { CS4, QUARTER },
    { C4, HALF },
    { 0, QUARTER },

    { G3, QUARTER },
    { AS3, QUARTER },
    { C4, HALF },
    { 0, QUARTER },

    { AS3, QUARTER },
    { G3, FULL },
    { 0, FULL },
};

static void setup(void) {

    return;
}


int main(void) {
    setup();

    return(0);
}

