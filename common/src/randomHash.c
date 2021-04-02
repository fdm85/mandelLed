/*
 * randomHash.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "ledData.h"
#include <stdint.h>
#include <stdlib.h>
#include "assrt.h"
#ifdef STM32F407xx
#include "rng.h"
#endif
#ifdef STM32L476xx
#include "rng.h"
#endif

typedef union
{
	uint32_t u32;
	struct
	{
		uint8_t a;
		uint8_t b;
		uint8_t c;
		uint8_t d;
	};
} rand_u;

// todo fix this, create factory macro to allocate
//Led_progColor_t __attribute__((section (".ccmram"))) prog_r23[619];
diffRunnerCtx_t *diff;
static uint16_t cycleMin_r23 = 100u;
static uint16_t it_r2 = 100u;

void anim_r23Init(LedChainDesc_t *const lcd)
{
	diff->lDc = malloc(sizeof(Led_progColor_t) * diff->size);
	for (uint32_t i = 0; i < diff->size; ++i)
	{
		diff->lDc[i].r = 0L;
		diff->lDc[i].g = 0L;
		diff->lDc[i].b = 0L;

		diff->lDc[i].rP = 0L;
		diff->lDc[i].gP = 0L;
		diff->lDc[i].bP = 0L;

		diff->lDc[i].itCur = 0u;
		diff->lDc[i].itMax = 0u;
	}
}

void anim_random1(LedChainDesc_t *const lcd)
{
	for (uint32_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{
		rand_u r;
		HAL_RNG_GenerateRandomNumber(&hrng, &r.u32);
		led_setLedToColor(lcd, i, r.a, r.b, r.c);
	}
}

void anim_setRandom2CycleCount(uint16_t c)
{
	cycleMin_r23 = c;
}

static void anim_Diff(LedChainDesc_t *const lcd, uint32_t i, bool isR3)
{
	rand_u r;
	LedLogic_t l;
	HAL_RNG_GenerateRandomNumber(&hrng, &r.u32);
	led_getLedColor(lcd, i, &l);

	_iq div;

	if (isR3)
	{
		diff->lDc[i].itCur = 0u;
		diff->lDc[i].itMax = r.d;

		if (diff->lDc[i].itMax == 0u)
		{
			++diff->lDc[i].itMax;
		}

		div = _IQ(diff->lDc[i].itMax);

	}
	else
	{
		div = _IQ(cycleMin_r23);
	}

	diff->lDc[i].r = _IQ(l.r);
	diff->lDc[i].g = _IQ(l.g);
	diff->lDc[i].b = _IQ(l.b);

	diff->lDc[i].rP = _IQdiv(_IQ((int16_t )r.a - l.r), div);
	diff->lDc[i].gP = _IQdiv(_IQ((int16_t )r.b - l.g), div);
	diff->lDc[i].bP = _IQdiv(_IQ((int16_t )r.c - l.b), div);
}

static void anim_render(LedChainDesc_t *const lcd, uint32_t i)
{

	diff->lDc[i].r += diff->lDc[i].rP;
	diff->lDc[i].g += diff->lDc[i].gP;
	diff->lDc[i].b += diff->lDc[i].bP;

	int32_t rOut = _IQint(diff->lDc[i].r);
	int32_t gOut = _IQint(diff->lDc[i].g);
	int32_t bOut = _IQint(diff->lDc[i].b);

	assrt(rOut <= UINT8_MAX);
	assrt(gOut <= UINT8_MAX);
	assrt(bOut <= UINT8_MAX);
	assrt(rOut >= 0L);
	assrt(gOut >= 0L);
	assrt(bOut >= 0L);

	led_setLedToColor(lcd, i, (uint8_t) rOut, (uint8_t) gOut, (uint8_t) bOut);
}
static void anim_r2Diff(LedChainDesc_t *const lcd)
{
	for (uint32_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{
		anim_Diff(lcd, i, false);
	}
}

static void anim_r2CalcAndSet(LedChainDesc_t *const lcd)
{
	for (uint32_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{
		anim_render(lcd, i);
	}
}

void anim_random2(LedChainDesc_t *const lcd)
{
	if (it_r2 == cycleMin_r23)
	{
		it_r2 = 0u;
		anim_r2Diff(lcd);
	}

	anim_r2CalcAndSet(lcd);
	++it_r2;
}

void anim_random3(LedChainDesc_t *const lcd)
{
	for (uint32_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{

		if (diff->lDc[i].itCur == diff->lDc[i].itMax)
		{
			anim_Diff(lcd, i, true);
		}

		anim_render(lcd, i);
		++diff->lDc[i].itCur;
	}
}
