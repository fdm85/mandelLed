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
	anim_enumAssrt
}anim_mode_e;

void anim_setCirc(bool shrt);
void anim_CyclicCall(void);
void anim_setMode(anim_mode_e set);
void anim_setBrightness(uint8_t set);
void anim_nextMode(void);
bool anim_Repeat(void);

void anim_circularRun1(uint8_t brightness);
void anim_random1(void);
void anim_circularRun2(uint8_t brightness);

void anim_setRandom2CycleCount(uint16_t c);
void anim_random2(void);
void anim_random3(void);
void anim_r23Init(void);

#endif /* ANIMATIONS_H_ */
