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

void anim_initRedRider(redRider_t* arg)
{
	arg->pos = 0uL;
	arg->ledStart = 0uL;
	arg->ledEnd = (uint32_t)D_LED_COUNT;
	arg->cycleCount = 1u;
	arg->brightness = 255u;
	arg->sig = false;
}

void anim_redRider(redRider_t* arg) {

	led_setLedToColor(arg->pos, arg->brightness/4u, 0u, 0u);
	led_setLedToColor((arg->pos + 1u), arg->brightness/3, 0u, 0u);
	led_setLedToColor((arg->pos + 2u), arg->brightness/2, 0u, 0u);
	led_setLedToColor((arg->pos + 3u), arg->brightness, 0u, 0u);
	led_setLedToColor((arg->pos + 4u), arg->brightness/2, 0u, 0u);
	led_setLedToColor((arg->pos + 5u), arg->brightness/3, 0u, 0u);
	led_setLedToColor((arg->pos + 6u), arg->brightness/4, 0u, 0u);

	if((arg->pos == arg->ledEnd) && !arg->sig)
	{
		arg->sig = true;
	}
	else if((arg->pos == arg->ledStart) && arg->sig)
	{
		arg->sig = false;
	}

	if(arg->sig) --arg->pos;
	else ++arg->pos;
}

