/*
 * animationsMain.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */


#include "animations.h"
#include "assrt.h"

static anim_mode_e currMode = anim_cR1;
static uint8_t brightness = 15u;

void anim_setMode(anim_mode_e set)
{
	currMode = set;
}

void anim_setBrightness(uint8_t set)
{
	brightness = set;
}

void anim_CyclicCall(void)
{
	switch (currMode) {
		case anim_cR1:
			anim_circularRun1(brightness);
			break;
		case anim_rnd1:
			anim_random1();
			break;
		case anim_rnd2:
			anim_random2();
			break;
		default:
			assrt(false);
			break;
	}
}
