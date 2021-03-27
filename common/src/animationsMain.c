/*
 * animationsMain.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "assrt.h"
#include "ledData.h"
#include "physic.h"
#include "matrix.h"
#include "msgeq7.h"

static anim_mode_e currMode = anim_cR2;
static uint8_t brightness = 255u;
static rider_t rider1;
static rider_t rider2;
static rider_t rider3;
static rider_t rPu01;
static rider_t rPu11;
static rider_t rPu02;
static rider_t rPu12;
static rider_t rPu03;
static rider_t rPu13;
static rider_t rPu04;
static rider_t rPu14;

rider_t *rA[] =
{ &rider1, &rider2, &rider3, NULL };

rider_t *rPu1[] =
{ &rPu01, &rPu11, NULL };
rider_t *rPu2[] =
{ &rPu02, &rPu12, NULL };
rider_t *rPu3[] =
{ &rPu03, &rPu13, NULL };
rider_t *rPu4[] =
{ &rPu04, &rPu14, NULL };

void anim_setMode(const LedChainDesc_t* lcd, anim_mode_e set)
{
	assrt(set < anim_enumAssrt);
	currMode = set;
	if ((set == anim_rnd2) || (set == anim_rnd3) || (set == anim_layers))
	{
		anim_r23Init(lcd);
	}
	if ((set == anim_cR2) || (set == anim_layers))
	{
		anim_initRedRider(lcd, &rider1);
		anim_initRedRider2(lcd, &rider2);
		anim_initRedRider3(lcd, &rider3);
	}
}

void anim_nextMode(const LedChainDesc_t* lcd)
{
	++currMode;

	if (currMode == anim_enumAssrt)
	{
		currMode = anim_cR1;
	}
	anim_r23Init(lcd);
	anim_initRedRider(lcd, &rider1);
	anim_initRedRider2(lcd, &rider2);
	anim_initRedRider3(lcd, &rider3);
}

void anim_setBrightness(uint8_t set)
{
	brightness = set;
}

void anim_addBrightness(int8_t add)
{

	int32_t new = brightness;
	new += add;
	if ((new > 0) && (new <= UINT8_MAX))
	{
		brightness = (uint8_t) new;
	}
	else if (add > 0)
	{
		brightness = UINT8_MAX;
	}
	else
	{
		brightness = 0u;
	}
}

#ifdef STM32F407xx
typedef enum
{
	init, blueSpawn, greenSpawn, redSpawn, whiteSpawn, fullWhite, done,
} puState_t;
static puState_t puS = init;
static void powerUp(const LedChainDesc_t* lcd)
{
	static uint8_t breaker = 0u;
	static uint32_t ledCount = 0uL;
	switch (puS)
	{
	case init:
		ledCount = getLedCount(lcd);
		led_setAllLedsToColor(lcd, 0, 0, 0);
		anim_initPuRide(lcd, &rPu01, 0u, 0u, 255u, 0u);
		anim_initPuRide(lcd, &rPu11, 0u, 0u, 255u, 1u);
		anim_initPuRide(lcd, &rPu02, 0u, 255u, 0u, 0u);
		anim_initPuRide(lcd, &rPu12, 0u, 255u, 0u, 1u);
		anim_initPuRide(lcd, &rPu03, 255u, 0u, 0u, 0u);
		anim_initPuRide(lcd, &rPu13, 255u, 0u, 0u, 1u);
		anim_initPuRide(lcd, &rPu04, 255u, 255u, 255u, 0u);
		anim_initPuRide(lcd, &rPu14, 255u, 255u, 255u, 1u);
		puS = blueSpawn;
		break;
	case blueSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(lcd, rPu1[i]);
			rideOnceFiller(lcd, rPu1[i]);
		}
		if (rPu01.iteration > 32u)
			puS = greenSpawn;
		break;
	case greenSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(lcd, rPu1[i]);
			rideOnceFiller(lcd, rPu1[i]);
			riderBlanker(lcd, rPu2[i]);
			rideOnceFiller(lcd, rPu2[i]);
		}
		if (rPu02.iteration > 32u)
			puS = redSpawn;
		break;
	case redSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(lcd, rPu1[i]);
			rideOnceFiller(lcd, rPu1[i]);
			riderBlanker(lcd, rPu2[i]);
			rideOnceFiller(lcd, rPu2[i]);
			riderBlanker(lcd, rPu3[i]);
			rideOnceFiller(lcd, rPu3[i]);
		}
		if (rPu03.iteration > 32u)
			puS = whiteSpawn;
		break;
	case whiteSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(lcd, rPu1[i]);
			rideOnceFiller(lcd, rPu1[i]);
			riderBlanker(lcd, rPu2[i]);
			rideOnceFiller(lcd, rPu2[i]);
			riderBlanker(lcd, rPu3[i]);
			rideOnceFiller(lcd, rPu3[i]);
			rideOnceFiller(lcd, rPu4[i]);
		}
		if (!rPu04.length)
			puS = fullWhite;
		break;
	case fullWhite:
	{
		LedLogic_t l;
		++breaker;
		for (uint32_t i = 0; i < ledCount; ++i) {
			led_getLedColor(lcd, i, &l);

			if( (l.r + 40u) < UINT8_MAX)
				l.r = (uint8_t)(l.r + 20u);
			else
				l.r = UINT8_MAX;

			if( (l.g + 40u) < UINT8_MAX)
				l.g = (uint8_t)(l.g + 20u);
			else
				l.g = UINT8_MAX;

			if( (l.b + 40u) < UINT8_MAX)
				l.b = (uint8_t)(l.b + 20u);
			else
				l.b = UINT8_MAX;

			led_setLedToColor(lcd, i, l.r, l.g, l.b);
		}
		if(breaker > 12)
			puS = done;
		break;
	}

	case done:
		return;
	default:
		__BKPT(0);
		break;
	}
}
#endif

static void layers(const LedChainDesc_t* lcd)
{
	anim_random3(lcd);
	for (uint8_t i = 0; rA[i + 1] != NULL; ++i)
	{
		if (phy_doesCollide(rA[i], rA[i + 1]))
		{
			phy_perfSimpleImpact(rA[i], rA[i + 1]);
			phy_swapColors(rA[i], rA[i + 1]);
		}
	}
	for (uint8_t i = 0; rA[i] != NULL; ++i)
	{
		riderBlanker(lcd, rA[i]);
	}
	for (uint8_t i = 0; rA[i] != NULL; ++i)
	{
		riderFiller(lcd, rA[i]);
	}
}

void anim_CyclicCall(const LedChainDesc_t* lcd)
{
#ifdef STM32F407xx
	if (puS != done)
	{
		powerUp(lcd);
		return;
	}
#endif
	switch (currMode)
	{
	case anim_cR1:
		anim_circularRun1(lcd, brightness);
		break;
	case anim_cR2:
		riderBlanker(lcd, &rider1);
		riderFiller(lcd, &rider1);
		break;
	case anim_rnd1:
		anim_random1(lcd);
		break;
	case anim_rnd2:
		anim_random2(lcd);
		break;
	case anim_rnd3:
		anim_random3(lcd);
		break;
	case anim_white:
		led_setAllLedsToColor(lcd, brightness, brightness, brightness);
		break;
	case anim_red:
		led_setAllLedsToColor(lcd, brightness, 0, 0);
		break;
	case anim_green:
		led_setAllLedsToColor(lcd, 0, brightness, 0);
		break;
	case anim_blue:
		led_setAllLedsToColor(lcd, 0, 0, brightness);
		break;
	case anim_cycleColors:
	{
		static uint8_t z = 0u;
		switch (z++)
		{
		case 0:
			led_setAllLedsToColor(lcd, brightness, brightness, brightness);
			break;
		case 1:
			led_setAllLedsToColor(lcd, brightness, 0, 0);
			break;
		case 2:
			led_setAllLedsToColor(lcd, 0, brightness, 0);
			break;
		case 3:
		default:
			led_setAllLedsToColor(lcd, 0, 0, brightness);
			z = 0u;
			break;
		}
		break;
	}
	case anim_layers:
		layers(lcd);
		break;
	case anim_SpecGraph:
	{
#ifdef STM32F303xE
		static uint8_t color = 80u;
		led_setAllLedsToColor(lcd, 15,15,15);
		mtrx_setLedsScaled(&left[0], getLChanVal(e63Hz), 0u, color, color);
		mtrx_setLedsScaled(&left[1], getLChanVal(e160Hz), color, 0u, color);
		mtrx_setLedsScaled(&left[2], getLChanVal(e400Hz), color, color, 0u);
		mtrx_setLedsScaled(&left[3], getLChanVal(e1kHz), 0u, color, 0u);
		mtrx_setLedsScaled(&left[4], getLChanVal(e2_5kHz), 0u, 0u, color);
		mtrx_setLedsScaled(&left[5], getLChanVal(e6_25kHz), color, 0u, 0u);
		mtrx_setLedsScaled(&left[6], getLChanVal(e16kHz), color, color, color);
		mtrx_setLedsScaled(&lright[0], getRChanVal(e63Hz), 0u, color, color);
		mtrx_setLedsScaled(&lright[1], getRChanVal(e160Hz), color, 0u, color);
		mtrx_setLedsScaled(&lright[2], getRChanVal(e400Hz), color, color, 0u);
		mtrx_setLedsScaled(&lright[3], getRChanVal(e1kHz), 0u, color, 0u);
		mtrx_setLedsScaled(&lright[4], getRChanVal(e2_5kHz), 0u, 0u, color);
		mtrx_setLedsScaled(&lright[5], getRChanVal(e6_25kHz), color, 0u, 0u);
		mtrx_setLedsScaled(&lright[6], getRChanVal(e16kHz), color, color, color);
#endif
		break;
	}
	default:
		assrt(false);
		break;
	}
}
