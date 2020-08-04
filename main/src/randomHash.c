/*
 * randomHash.c
 *
 *  Created on: 01.08.2020
 *      Author: gruenberger
 */

#include "animations.h"
#include "ledData.h"
#include "crc.h"
#include "rng.h"
#include <stdint.h>
#include "assrt.h"

typedef union {
	uint32_t u32;
	struct {
		uint8_t a;
		uint8_t b;
		uint8_t c;
		uint8_t d;
	};
} rand_u;

Led_diffColor_t diffs_r2[ledCount];
uint16_t cycle_r2 = 100u;
uint16_t it_r2 = 0u;

void anim_random1(void) {
	for (uint32_t i = 0; i < led_count; ++i) {
		rand_u r;
		HAL_RNG_GenerateRandomNumber(&hrng, &r.u32);
		led_setLedToColor(i, r.a, r.b, r.c);
	}
}

void anim_setRandom2CycleCount(uint16_t c) {
	cycle_r2 = c;
}

static void anim_r2Diff(void) {
	it_r2 = 0u;
	for (uint32_t i = 0; i < led_count; ++i) {
		rand_u r;
		Led_Led_t l;

		HAL_RNG_GenerateRandomNumber(&hrng, &r.u32);
		led_getLedColor(i, &l);

		diffs_r2[i].r = _IQdiv(_IQ((int16_t )l.r - r.a), _IQ(cycle_r2));
		diffs_r2[i].g = _IQdiv(_IQ((int16_t )l.g - r.b), _IQ(cycle_r2));
		diffs_r2[i].b = _IQdiv(_IQ((int16_t )l.b - r.c), _IQ(cycle_r2));
	}
}

static void anim_r2CalcAndSet(void) {
	for (uint32_t i = 0; i < led_count; ++i) {
		Led_Led_t in;
		Led_Led_t out;
		led_getLedColor(i, &in);

		_iq it = _IQ(it_r2);
		_iq r = _IQmpy(diffs_r2[i].r, it);
		_iq g = _IQmpy(diffs_r2[i].g, it);
		_iq b = _IQmpy(diffs_r2[i].b, it);

		int32_t rOut = in.r + _IQint(r);
		int32_t gOut = in.g + _IQint(g);
		int32_t bOut = in.b + _IQint(b);

		assrt(rOut < UINT8_MAX);
		assrt(gOut < UINT8_MAX);
		assrt(bOut < UINT8_MAX);

		out.r = (uint8_t)rOut;
		out.g = (uint8_t)gOut;
		out.b = (uint8_t)bOut;
		led_setLedToColor(i, out.r, out.g, out.b);
	}
}

void anim_random2(void) {
	if (it_r2 == cycle_r2) {
		anim_r2Diff();
	}

	anim_r2CalcAndSet();
	++cycle_r2;
}
