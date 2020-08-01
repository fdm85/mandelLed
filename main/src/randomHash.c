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

typedef union {
	uint32_t u32;
	struct {
		uint8_t a;
		uint8_t b;
		uint8_t c;
		uint8_t d;
	};
}rand_u;

void anim_random1(void)
{
	for (uint32_t i = 0; i < led_count; ++i) {
		rand_u r;
		HAL_RNG_GenerateRandomNumber(&hrng, &r.u32);
		led_setLedToColor(i, r.a, r.b, r.c);
	}
}
