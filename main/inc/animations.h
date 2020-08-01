/*
 * animations.h
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdint.h>

typedef enum  {
	anim_cR1 = 0,
	anim_rnd1,
}anim_mode_e;

void anim_CyclicCall(void);
void anim_setMode(anim_mode_e set);
void anim_setBrightness(uint8_t set);

void anim_circularRun1(uint8_t brightness);
void anim_random1(void);

#endif /* ANIMATIONS_H_ */
