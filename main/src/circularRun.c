/*
 * circularRun.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "ledData.h"

void circularRun1(uint8_t brightness)
{
	static uint32_t index = 0u;
	led_setAllLedsToUniColors(brightness);
	led_setLedToColor(index, 0u,0u, brightness);
	led_setLedToColor((index + 1u), 0u,brightness, 0u);
	led_setLedToColor((index + 2u), brightness,0u, 0u);
	led_setLedToColor(index + 20u, 0u,0u, brightness);
	led_setLedToColor((index + 21u), 0u,brightness, 0u);
	led_setLedToColor((index + 22u), brightness,0u, 0u);
	led_setLedToColor(index + 40u, 0u,0u, brightness);
	led_setLedToColor((index + 41u), 0u,brightness, 0u);
	led_setLedToColor((index + 42u), brightness,0u, 0u);

	++index;
}
