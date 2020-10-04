/*
 * animationsMain.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "assrt.h"
#include "ledData.h"

static anim_mode_e currMode = anim_white;
static uint8_t brightness = 255u;

static bool repeat = true;

void anim_setMode(anim_mode_e set) {
	assrt(set < anim_enumAssrt);
	currMode = set;
	if ((set == anim_rnd2) || (set == anim_rnd3)) {
		anim_r23Init();
	}
}

void anim_nextMode(void) {
	++currMode;

	if (currMode == anim_enumAssrt) {
		currMode = anim_cR1;
	}
	anim_r23Init();

	repeat = true;
}

bool anim_needRepeat(void)
{
	return repeat;
}

void anim_setBrightness(uint8_t set) {
	brightness = set;
	repeat = true;
}

void anim_addBrightness(int8_t add) {

	int32_t new = brightness;
	new += add;
	if( (new > 0) && (new <= UINT8_MAX) )
	{
		brightness = (uint8_t)new;
	}
	else if(add > 0)
	{
		brightness = UINT8_MAX;
	}
	else
	{
		brightness = 0u;
	}
	repeat = true;
}

void anim_CyclicCall(void) {
	switch (currMode) {
	case anim_cR1:
		anim_circularRun1(brightness);
		break;
	case anim_cR2:
		anim_circularRun2(brightness);
		break;
	case anim_rnd1:
		anim_random1();
		break;
	case anim_rnd2:
		anim_random2();
		break;
	case anim_rnd3:
		anim_random3();
		break;
	case anim_white:
		led_setAllLedsToColor(brightness, brightness, brightness);
		break;
	case anim_red:
		led_setAllLedsToColor(brightness, 0, 0);
		repeat = true;
		break;
	case anim_green:
		led_setAllLedsToColor(0, brightness, 0);
		repeat = true;
		break;
	case anim_blue:
		led_setAllLedsToColor(0, 0, brightness);
		repeat = true;
		break;
	case anim_cycleColors:
	{
		static uint8_t z = 0u;
		switch(z++)
		{
		case 0:
			led_setAllLedsToColor(brightness, brightness, brightness);
			break;
		case 1:
			led_setAllLedsToColor(brightness, 0, 0);
			break;
		case 2:
			led_setAllLedsToColor(0, brightness, 0);
			break;
		case 3:
		default:
			led_setAllLedsToColor(0, 0, brightness);
			z = 0u;
			break;
		}
		break;
	}
	default:
		assrt(false);
		break;
	}
}
