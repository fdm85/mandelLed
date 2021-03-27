/*
 * randomHash.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "ledData.h"
#include <stdint.h>
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

Led_progColor_t __attribute__((section (".ccmram"))) prog_r23[D_LED_COUNT];
static uint16_t cycleMin_r23 = 100u;
static uint16_t it_r2 = 100u;
static uint8_t dimMult = 3u;
static uint8_t dimDiv = 4u;
static _iq dimFactor;

void anim_r23Init(void)
{
	for (uint32_t i = 0; i < led_count; ++i)
	{
		prog_r23[i].r = 0L;
		prog_r23[i].g = 0L;
		prog_r23[i].b = 0L;

		prog_r23[i].rP = 0L;
		prog_r23[i].gP = 0L;
		prog_r23[i].bP = 0L;

		prog_r23[i].itCur = 0u;
		prog_r23[i].itMax = 0u;
	}
	dimFactor = _IQdiv(dimMult, dimDiv);
}

void anim_random1(void)
{
	for (uint32_t i = 0; i < led_count; ++i)
	{
		rand_u r;
		HAL_RNG_GenerateRandomNumber(&hrng, &r.u32);
		led_setLedToColor(i, r.a, r.b, r.c);
	}
}

void anim_setRandom2CycleCount(uint16_t c)
{
	cycleMin_r23 = c;
}

static void anim_Diff(uint32_t i, bool isR3)
{
	rand_u r;
	Led_Led_t l;
	HAL_RNG_GenerateRandomNumber(&hrng, &r.u32);
	led_getLedColor(i, &l);

	_iq div;

	if (isR3)
	{
		prog_r23[i].itCur = 0u;
		prog_r23[i].itMax = r.d;

		if (prog_r23[i].itMax == 0u)
		{
			++prog_r23[i].itMax;
		}

		div = _IQ(prog_r23[i].itMax);

	}
	else
	{
		div = _IQ(cycleMin_r23);
	}

	prog_r23[i].r = _IQ(l.r);
	prog_r23[i].g = _IQ(l.g);
	prog_r23[i].b = _IQ(l.b);

	prog_r23[i].rP = _IQdiv(_IQ((int16_t )r.a - l.r), div);
	prog_r23[i].gP = _IQdiv(_IQ((int16_t )r.b - l.g), div);
	prog_r23[i].bP = _IQdiv(_IQ((int16_t )r.c - l.b), div);
}

static void anim_render(uint32_t i)
{

	prog_r23[i].r += prog_r23[i].rP;
	prog_r23[i].g += prog_r23[i].gP;
	prog_r23[i].b += prog_r23[i].bP;

	int32_t rOut = _IQint(prog_r23[i].r);
	int32_t gOut = _IQint(prog_r23[i].g);
	int32_t bOut = _IQint(prog_r23[i].b);

	assrt(rOut <= UINT8_MAX);
	assrt(gOut <= UINT8_MAX);
	assrt(bOut <= UINT8_MAX);
	assrt(rOut >= 0L);
	assrt(gOut >= 0L);
	assrt(bOut >= 0L);

	led_setLedToColor(i, (uint8_t) rOut, (uint8_t) gOut, (uint8_t) bOut);
}
static void anim_r2Diff(void)
{
	for (uint32_t i = 0; i < led_count; ++i)
	{
		anim_Diff(i, false);
	}
}

static void anim_r2CalcAndSet(void)
{
	for (uint32_t i = 0; i < led_count; ++i)
	{
		anim_render(i);
	}
}

void anim_random2(void)
{
	if (it_r2 == cycleMin_r23)
	{
		it_r2 = 0u;
		anim_r2Diff();
	}

	anim_r2CalcAndSet();
	++it_r2;
}

void anim_random3(void)
{
	for (uint32_t i = 0; i < led_count; ++i)
	{

		if (prog_r23[i].itCur == prog_r23[i].itMax)
		{
			anim_Diff(i, true);
		}

		anim_render(i);
		++prog_r23[i].itCur;
	}
}
