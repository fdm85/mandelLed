/*
 * circularRun.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "ledData.h"
#include "stdbool.h"

#define cycle 10u
static uint8_t cycleCount = cycle;

void anim_circularRun1(uint8_t brightness) {
	static uint32_t index = 0u;
	if (!cycleCount) {

		led_setAllLedsToUniColors(brightness);
		led_setLedToColor(index, 0u, 0u, brightness);
		led_setLedToColor((index + 1u), 0u, brightness, 0u);
		led_setLedToColor((index + 2u), brightness, 0u, 0u);
		led_setLedToColor(index + 20u, 0u, 0u, brightness);
		led_setLedToColor((index + 21u), 0u, brightness, 0u);
		led_setLedToColor((index + 22u), brightness, 0u, 0u);
		led_setLedToColor(index + 40u, 0u, 0u, brightness);
		led_setLedToColor((index + 41u), 0u, brightness, 0u);
		led_setLedToColor((index + 42u), brightness, 0u, 0u);

		++index;
		cycleCount = cycle;
	} else {
		--cycleCount;
	}
}

void anim_circularRun2(uint8_t brightness) {
	static uint32_t index = 0u;
	static bool direction = false;
	static int8_t dir = 1;
	if (!cycleCount) {

		led_setAllLedsToUniColors(brightness);
		led_setLedToColor(index, 0u, 0u, brightness);
		led_setLedToColor((index + 1u), 0u, brightness, 0u);
		led_setLedToColor((index + 2u), brightness, 0u, 0u);
		led_setLedToColor(index + 20u, 0u, 0u, brightness);
		led_setLedToColor((index + 21u), 0u, brightness, 0u);
		led_setLedToColor((index + 22u), brightness, 0u, 0u);
		led_setLedToColor(index + 40u, 0u, 0u, brightness);
		led_setLedToColor((index + 41u), 0u, brightness, 0u);
		led_setLedToColor((index + 42u), brightness, 0u, 0u);



		++index;
		cycleCount = cycle;
	} else {
		--cycleCount;
	}
}
