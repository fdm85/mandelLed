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
	anim_enumAssrt
}anim_mode_e;

typedef enum
{
	init, blueSpawn, greenSpawn, redSpawn, whiteSpawn, fullWhite, done,
} puState_t;

typedef enum
{
	e_render, e_waitTxCplt, e_paste
} eSm;

typedef struct mAnim_tag mAnim_t;
typedef void (*fpRender)(mAnim_t* ctx);
struct mAnim_tag
{
	fpRender fpRend;
	LedChainDesc_t *const lcd_ctx;
	const uint32_t triggerTimeMs;
	uint32_t lastToggle;
	volatile uint32_t sendLock;
	volatile uint32_t a, b, c, d, e, f;
	eSm state;
	puState_t puState;
	uint16_t padd2;
};

void anim_setCirc(bool shrt);
void anim_CyclicCall(mAnim_t* ctx);
void anim_setMode(LedChainDesc_t *const lcd, anim_mode_e set);
void anim_setBrightness(uint8_t set);
void anim_addBrightness(int8_t add);
void anim_nextMode(LedChainDesc_t *const lcd);

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

#if !(defined(STM32F103xB))
void anim_circularRun1(LedChainDesc_t *const lcd, uint8_t brightness);

void anim_initPuRide(LedChainDesc_t *const lcd, rider_t* arg, uint8_t r, uint8_t g, uint8_t b, uint8_t dir);
void rideOnceFiller(LedChainDesc_t *const lcd, rider_t* arg);
void riderBlanker(LedChainDesc_t *const lcd, rider_t* arg);
void riderFiller(LedChainDesc_t *const lcd, rider_t* arg);
void anim_initRedRider(LedChainDesc_t *const lcd, rider_t* arg);
void anim_initRedRider2(LedChainDesc_t *const lcd, rider_t* arg);
void anim_initRedRider3(LedChainDesc_t *const lcd, rider_t* arg);

#else
#define anim_initPuRide(a,b,c,d,e,f)
#define rideOnceFiller(a,b)
#define riderBlanker(a,b)
#define riderFiller(a,b)
#define anim_initRedRider(a,b)
#define anim_initRedRider2(a,b)
#define anim_initRedRider3(a,b)
#define anim_circularRun1(a,b)

#endif


#if !( (defined(STM32F303xE) | defined(STM32F103xB)) )
void anim_random1(LedChainDesc_t *const lcd);
void anim_setRandom2CycleCount(uint16_t c);
void anim_random2(LedChainDesc_t *const lcd);
void anim_random3(LedChainDesc_t *const lcd);
void anim_r23Init(LedChainDesc_t *const lcd);
void anim_r23DeInit(LedChainDesc_t *const lcd);
void anim_layerRedRider(uint32_t pos);

#else
#define anim_random1(x)
#define anim_setRandom2CycleCount(x)
#define anim_random2(x)
#define anim_random3(x)
#define anim_r23Init(x)

#define anim_r23DeInit(x)
#define anim_layerRedRider(x)

#endif

#endif /* ANIMATIONS_H_ */
