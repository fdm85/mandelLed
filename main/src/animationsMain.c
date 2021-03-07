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

void anim_setMode(anim_mode_e set)
{
	assrt(set < anim_enumAssrt);
	currMode = set;
	if ((set == anim_rnd2) || (set == anim_rnd3) || (set == anim_layers))
	{
		anim_r23Init();
	}
	if ((set == anim_cR2) || (set == anim_layers))
	{
		anim_initRedRider(&rider1);
		anim_initRedRider2(&rider2);
		anim_initRedRider3(&rider3);
	}
}

void anim_nextMode(void)
{
	++currMode;

	if (currMode == anim_enumAssrt)
	{
		currMode = anim_cR1;
	}
	anim_r23Init();
	anim_initRedRider(&rider1);
	anim_initRedRider2(&rider2);
	anim_initRedRider3(&rider3);
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

typedef enum
{
	init, blueSpawn, greenSpawn, redSpawn, whiteSpawn, fullWhite, done,
} puState_t;
static puState_t puS = init;

static void powerUp(void)
{
	static uint8_t breaker = 0u;
	static uint32_t ledCount = 0uL;
	switch (puS)
	{
	case init:
		ledCount = getLedCount();
		led_setAllLedsToColor(0, 0, 0);
		anim_initPuRide(&rPu01, 0u, 0u, 255u, 0u);
		anim_initPuRide(&rPu11, 0u, 0u, 255u, 1u);
		anim_initPuRide(&rPu02, 0u, 255u, 0u, 0u);
		anim_initPuRide(&rPu12, 0u, 255u, 0u, 1u);
		anim_initPuRide(&rPu03, 255u, 0u, 0u, 0u);
		anim_initPuRide(&rPu13, 255u, 0u, 0u, 1u);
		anim_initPuRide(&rPu04, 255u, 255u, 255u, 0u);
		anim_initPuRide(&rPu14, 255u, 255u, 255u, 1u);
		puS = blueSpawn;
		break;
	case blueSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(rPu1[i]);
			rideOnceFiller(rPu1[i]);
		}
		if (rPu01.iteration > 32u)
			puS = greenSpawn;
		break;
	case greenSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(rPu1[i]);
			rideOnceFiller(rPu1[i]);
			riderBlanker(rPu2[i]);
			rideOnceFiller(rPu2[i]);
		}
		if (rPu02.iteration > 32u)
			puS = redSpawn;
		break;
	case redSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(rPu1[i]);
			rideOnceFiller(rPu1[i]);
			riderBlanker(rPu2[i]);
			rideOnceFiller(rPu2[i]);
			riderBlanker(rPu3[i]);
			rideOnceFiller(rPu3[i]);
		}
		if (rPu03.iteration > 32u)
			puS = whiteSpawn;
		break;
	case whiteSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(rPu1[i]);
			rideOnceFiller(rPu1[i]);
			riderBlanker(rPu2[i]);
			rideOnceFiller(rPu2[i]);
			riderBlanker(rPu3[i]);
			rideOnceFiller(rPu3[i]);
			rideOnceFiller(rPu4[i]);
		}
		if (!rPu04.length)
			puS = fullWhite;
		break;
	case fullWhite:
	{
		Led_Led_t l;
		++breaker;
		for (uint32_t i = 0; i < ledCount; ++i) {
			led_getLedColor(i, &l);

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

			led_setLedToColor(i, l.r, l.g, l.b);
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

static void layers(void)
{
	anim_random3();
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
		riderBlanker(rA[i]);
	}
	for (uint8_t i = 0; rA[i] != NULL; ++i)
	{
		riderFiller(rA[i]);
	}
}

void anim_CyclicCall(void)
{
//	if (puS != done)
//	{
//		powerUp();
//		return;
//	}
	switch (currMode)
	{
	case anim_cR1:
		anim_circularRun1(brightness);
		break;
	case anim_cR2:
		riderBlanker(&rider1);
		riderFiller(&rider1);
//		led_setAllLedsToColor(5, 0, 0);
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
		break;
	case anim_green:
		led_setAllLedsToColor(0, brightness, 0);
		break;
	case anim_blue:
		led_setAllLedsToColor(0, 0, brightness);
		break;
	case anim_cycleColors:
	{
		static uint8_t z = 0u;
		switch (z++)
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
	case anim_layers:
		layers();
		break;
	case anim_SpecGraph:
	{
		static uint8_t color = 1u;
		static uint8_t count = 0u;
		led_setAllLedsToColor(0,0,0);
		mtrx_setLeds(&left.bar, color, color, color);
//		led_setLedToColor(count, color, color, color);
		++count;
		if(count > 100u)
		{
			count = 0u;
			color = (uint8_t)( (0xFF & (color << 1)) + 1u);
			if(color > 253)
				color = 1u;
		}

		break;
	}
	default:
		assrt(false);
		break;
	}
}
