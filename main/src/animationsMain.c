/*
 * animationsMain.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "assrt.h"

static anim_mode_e currMode = anim_white;
static uint8_t brightness = 255u;

static bool repeat = false;

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

bool anim_Repeat(void)
{
	return repeat;
}

void anim_setBrightness(uint8_t set) {
	brightness = set;
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
		repeat = false;
		break;
	case anim_green:
		led_setAllLedsToColor(0, brightness, 0);
		repeat = false;
		break;
	case anim_blue:
		led_setAllLedsToColor(0, 0, brightness);
		repeat = false;
		break;
	default:
		assrt(false);
		break;
	}
}
