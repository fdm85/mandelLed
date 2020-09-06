/*
 * circularRun.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "ledData.h"

#define cycle 3u
#define cycleShort 3u
//static uint8_t cycleCount = cycle;
static uint8_t cycleCount = cycleShort;

void anim_setCirc(bool shrt)
{
	cycleCount = (shrt) ? cycleShort : cycle;
}

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
		led_setLedToColor(index, brightness/4u, 0u, 0u);
		led_setLedToColor((index + 1u), brightness/3, 0u, 0u);
		led_setLedToColor((index + 2u), brightness/2, 0u, 0u);
		led_setLedToColor((index + 3u), brightness, 0u, 0u);
		led_setLedToColor((index + 4u), brightness/2, 0u, 0u);
		led_setLedToColor((index + 5u), brightness/3, 0u, 0u);
		led_setLedToColor((index + 6u), brightness/4, 0u, 0u);

		if((index == (led_count - 7u)) && !direction)
		{
			dir = -1;
			direction = true;
		}
		else if((index == 0u) && direction)
		{
			dir = 1;
			direction = false;
		}
		else
		{
			index += dir;
		}


		cycleCount = cycle;
	} else {
		--cycleCount;
	}
}
