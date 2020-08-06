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

Led_progColor_t __attribute__((section (".ccmram"))) prog_r2[ledCount];
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

		prog_r2[i].r = _IQ(l.r);
		prog_r2[i].g = _IQ(l.g);
		prog_r2[i].b = _IQ(l.b);

		prog_r2[i].rP = _IQdiv(_IQ((int16_t )r.a - l.r), _IQ(cycle_r2));
		prog_r2[i].gP = _IQdiv(_IQ((int16_t )r.b - l.g), _IQ(cycle_r2));
		prog_r2[i].bP = _IQdiv(_IQ((int16_t )r.c - l.b), _IQ(cycle_r2));
	}
}

static void anim_r2CalcAndSet(void) {
	for (uint32_t i = 0; i < led_count; ++i) {
		Led_Led_t out;

		_iq r = prog_r2[i].r + prog_r2[i].rP;
		_iq g = prog_r2[i].g + prog_r2[i].gP;
		_iq b = prog_r2[i].b + prog_r2[i].bP;

		int32_t rOut = _IQint(r);
		int32_t gOut = _IQint(g);
		int32_t bOut = _IQint(b);

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
