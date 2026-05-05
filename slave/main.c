#include "mcu.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "bit_ops.h"
#include "timer0.h"
#include "timer1.h"
#include "tune.h"
#include "spi_slave.h"

#define LED_MOVE_PIN        PIN4
#define LED_DOOR_OPEN_PIN   PIN5
#define LED_DOOR_CLOSE_PIN  PIN6
#define LED_OBSTACLE_PIN    PIN7

// Non-stop background melody: Smoke on the Water (Deep Purple)
// Author: Henri Ilves
note_t smokeInTheWater[] = {
	{G3, HALF},
	{AS3, HALF},
	{C4, FULL},
	{0, QUARTER},

	{G3, HALF},
	{AS3, HALF},
	{CS4, QUARTER},
	{C4, FULL},
	{0, QUARTER},

	{G3, HALF},
	{AS3, HALF},
	{C4, FULL},
	{0, QUARTER},

	{AS3, HALF},
	{G3, FULL},
	{0, FULL},
};

#define smokeInTheWater_SIZE (sizeof(smokeInTheWater) / sizeof(note_t))

// Author: Henri Ilves
// Obstacle melody: Seven Nation Army (The White Stripes)
static note_t sevenNationArmy[] = {
	{E3, FULL},
	{0,  QUARTER},
	{E3, QUARTER},
	{G3, QUARTER + QUARTER/2},
	{E3, QUARTER + QUARTER/2},
	{D3, QUARTER + QUARTER/2},
	{C3, FULL},
	{B2, FULL},
	{0,  HALF},
};

#define OBSTACLE_MELODY_SIZE (sizeof(sevenNationArmy) / sizeof(note_t))


// 1 = obstacle melody is active
static volatile uint8_t buzzer_override = 0;

// Non-stop buzzer tick
// Author: Henri Ilves
void nonStop_tick(uint32_t ticks)
{
	if (buzzer_override) return;

	static uint32_t bg_note_ticks  = 0;
	static uint8_t  bg_note_index  = 0;

	uint32_t note_duration_ticks = smokeInTheWater[bg_note_index].duration_ms / 10;

	if (ticks - bg_note_ticks >= note_duration_ticks) {
		bg_note_index = (bg_note_index + 1) % smokeInTheWater_SIZE;
		bg_note_ticks = ticks;

		note_t note = smokeInTheWater[bg_note_index];
		if (note.frequency_hz == 0) {
			timer1_channel_A_off();
		} else {
			timer1_set_frequency(note.frequency_hz);
			timer1_channel_A_on();
		}
	}
}

// Obstacle detection
// Author: Henri Ilves
static void play_obstacle_melody(void)
{
	// Pause non-stop buzzer
	buzzer_override = 1;
	timer1_channel_A_off();

	// Blink LED_OBSTACLE three times
	for (uint8_t i = 0; i < 3; i++) {
		SET_BIT(PORTD, LED_OBSTACLE_PIN);
		_delay_ms(300);
		CLEAR_BIT(PORTD, LED_OBSTACLE_PIN);
		_delay_ms(300);
	}

	// Play melody
	uint8_t note_idx = 0;
	state_t current_state = OBSTACLE_DETECTION;

	while (current_state == OBSTACLE_DETECTION) {
		note_t note = sevenNationArmy[note_idx];

		if (note.frequency_hz == 0) {
			timer1_channel_A_off();
			} else {
			timer1_set_frequency(note.frequency_hz);
			timer1_channel_A_on();
		}

		uint32_t note_end = get_time() + note.duration_ms / 10;
		while (get_time() < note_end) {
			current_state = spi_slave_get_state();
			if (current_state != OBSTACLE_DETECTION) break;
		}

		note_idx = (note_idx + 1) % OBSTACLE_MELODY_SIZE;
	}

	timer1_channel_A_off();
	CLEAR_BIT(PORTD, LED_OBSTACLE_PIN);

	// Resume Smoke In The Water
	buzzer_override = 0;
}

int main(void)
{
	setup_timer0();
	setup_timer1();
	spi_slave_init();
	sei();

	// Start Smoke In The Water
	timer1_set_frequency(smokeInTheWater[0].frequency_hz);
	timer1_channel_A_on();

	while (1) {
		state_t state = spi_slave_get_state();

		// Movement LED
		// Author: Olli Kirkkopelto
		if (state == GOING_UP || state == GOING_DOWN) {
			SET_BIT(PORTD, LED_MOVE_PIN);
			} else {
			CLEAR_BIT(PORTD, LED_MOVE_PIN);
		}

		// Door opening LED
		// Author: Juho Koski
		if(state == DOOR_OPENING){
			SET_BIT(PORTD, LED_DOOR_OPEN_PIN);
		} else {
			CLEAR_BIT(PORTD, LED_DOOR_OPEN_PIN);
		}

		// Door closing LED
		// Author: Juho Koski
		if(state == DOOR_CLOSING){
			SET_BIT(PORTD,LED_DOOR_CLOSE_PIN);
		} else {
			CLEAR_BIT(PORTD, LED_DOOR_CLOSE_PIN);
		}

		// Author: Henri Ilves
		if (state == OBSTACLE_DETECTION) {
			play_obstacle_melody();
		}
	}

	return 0;
}
