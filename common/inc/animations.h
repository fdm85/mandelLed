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
	anim_powerUp = 0,
	anim_cR1,
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
	anim_SpecGraph,
	anim_enumAssrt
}anim_mode_e;

void anim_setCirc(bool shrt);
void anim_CyclicCall(const LedChainDesc_t* lcd);
void anim_setMode(const LedChainDesc_t* lcd, anim_mode_e set);
void anim_setBrightness(uint8_t set);
void anim_addBrightness(int8_t add);
void anim_nextMode(const LedChainDesc_t* lcd);

void anim_circularRun1(const LedChainDesc_t* lcd, uint8_t brightness);

typedef struct rider rider_t;
typedef void (*riderInit)(rider_t*);
struct rider
{
	riderInit fpInit;
	uint32_t pos;
	_iq posIq;
	uint32_t posMin;
	uint32_t posMax;
	_iq step;
	LedLogic_t c;
	uint8_t length;
	uint8_t blanks;
	uint8_t iteration;
	uint16_t padd;
};


void anim_initPuRide(const LedChainDesc_t* lcd, rider_t* arg, uint8_t r, uint8_t g, uint8_t b, uint8_t dir);
void rideOnceFiller(const LedChainDesc_t* lcd, rider_t* arg);
void riderBlanker(const LedChainDesc_t* lcd, rider_t* arg);
void riderFiller(const LedChainDesc_t* lcd, rider_t* arg);
void anim_initRedRider(const LedChainDesc_t* lcd, rider_t* arg);
void anim_initRedRider2(const LedChainDesc_t* lcd, rider_t* arg);
void anim_initRedRider3(const LedChainDesc_t* lcd, rider_t* arg);

#ifndef STM32F303xE
void anim_random1(const LedChainDesc_t* lcd);
void anim_setRandom2CycleCount(uint16_t c);
void anim_random2(const LedChainDesc_t* lcd);
void anim_random3(const LedChainDesc_t* lcd);
void anim_r23Init(const LedChainDesc_t* lcd);

void anim_layerRedRider(uint32_t pos);

#else
#define anim_random1(x)
#define anim_setRandom2CycleCount(x)
#define anim_random2(x)
#define anim_random3(x)
#define anim_r23Init(x)

#define anim_layerRedRider(x)

#endif

#endif /* ANIMATIONS_H_ */
