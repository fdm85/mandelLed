/*
 * circularRun.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "ledData.h"
#include "IQmathLib.h"

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

void anim_initRedRider(rider_t* arg)
{
	arg->pos = 0uL;
	arg->posMin = 0uL;
	arg->posMax = (uint32_t)D_LED_COUNT;
	arg->c.r = 255u;
	arg->c.g = 0u;
	arg->c.b = 0u;
	arg->step = _IQG(2);
	arg->length = 8u;
	arg->blanks = 4u;
	arg->posIq = _IQG(arg->pos);
}

void anim_initRedRider2(rider_t* arg)
{
	arg->posMin = 0uL;
	arg->posMax = (uint32_t)D_LED_COUNT;
	arg->pos = arg->posMax/2uL;
	arg->c.r = 100u;
	arg->c.g = 200u;
	arg->c.b = 0u;
	arg->step = _IQG(3.25);
	arg->length = 6u;
	arg->blanks = 4u;
	arg->posIq = _IQG(arg->pos);
}

void anim_initRedRider3(rider_t* arg)
{
	arg->posMin = 0uL;
	arg->posMax = (uint32_t)D_LED_COUNT;
	arg->c.r = 0u;
	arg->c.g = 0u;
	arg->c.b = 255u;
	arg->step = _IQG(-1.5);
	arg->length = 8u;
	arg->pos = arg->posMax - arg->length;
	arg->blanks = 4;
	arg->posIq = _IQG(arg->pos);
}

void riderBlanker(rider_t* arg)
{
	for (uint8_t i = 1; i <= arg->blanks; ++i) {

		if((int32_t)arg->pos -i >= (int32_t)arg->posMin)
			led_setLedToColor(arg->pos - i, 0u, 0u, 0u);
	}
	for (uint8_t i = 0; i < arg->blanks; ++i) {

		if(arg->pos + (uint32_t)(arg->length + i) <= arg->posMax)
			led_setLedToColor(arg->pos + (uint32_t)(arg->length + i), 0u, 0u, 0u);
	}
}

void riderFiller(rider_t* arg)
{
	for (uint8_t i = 0; i < arg->length; ++i) {
		led_setLedToColor((arg->pos + i), arg->c.r, arg->c.g, arg->c.b);
	}
	if((arg->pos >= (arg->posMax - arg->length)) && (arg->step > 0))
	{
		arg->step *= -1;
	}
	if((arg->pos <= (arg->posMin + arg->length)) && (arg->step < 0))
	{
		arg->step *= -1;
	}

	arg->posIq += arg->step;

	arg->pos = (uint32_t)_IQint(arg->posIq);

}


