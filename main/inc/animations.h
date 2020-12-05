/*
 * animations.h
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdint.h>
#include <stdbool.h>
#include "ledData.h"

typedef enum  {
	anim_cR1 = 0,
	anim_cR2,
	anim_rnd1,
	anim_rnd2,
	anim_rnd3,
	anim_white,
	anim_red,
	anim_green,
	anim_blue,
	anim_cycleColors,
	anim_layers,
	anim_enumAssrt
}anim_mode_e;

void anim_setCirc(bool shrt);
void anim_CyclicCall(void);
void anim_setMode(anim_mode_e set);
void anim_setBrightness(uint8_t set);
void anim_addBrightness(int8_t add);
void anim_nextMode(void);

void anim_circularRun1(uint8_t brightness);
void anim_random1(void);

typedef struct rider
{
	uint32_t pos;
	uint32_t ledStart;
	uint32_t ledEnd;
	Led_Led_t c;
	uint8_t step;
	uint8_t length;
	uint8_t blanks;
	bool sig;
}rider_t;

void riderBlanker(rider_t* arg);
void riderFiller(rider_t* arg);
void anim_initRedRider(rider_t* arg);
void anim_initRedRider2(rider_t* arg);
void anim_initRedRider3(rider_t* arg);

void anim_setRandom2CycleCount(uint16_t c);
void anim_random2(void);
void anim_random3(void);
void anim_r23Init(void);

void anim_layerRedRider(uint32_t pos);

#endif /* ANIMATIONS_H_ */
