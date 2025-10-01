/**
 * @file      randomHash.c
 * @authors   Clemens Grünberger
 * @copyright 2022  Clemens Grünberger
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @brief Random animation implementation
 * @defgroup Random_Anim Animations featuring chaotic (randomized) behavior
 * @ingroup Animations
 * @{
 */

#include "animations.h"
#include "ledData.h"
#include <stdint.h>
#include <stdlib.h>
#include "assrt.h"
#ifdef STM32F407xx
#include "rng.h"
#else
extern uint32_t hrng;
#endif
#ifdef STM32L476xx
#include "rng.h"
#endif

typedef union
{
	uint32_t u32; /*!< */
	struct
	{
		uint8_t a; /*!< */
		uint8_t b; /*!< */
		uint8_t c; /*!< */
		uint8_t d; /*!< */
	};
} rand_u;

static uint16_t cycleMin_r23 = 100u;
static uint16_t it_r2 = 100u;
#if !(defined(STM32F103xB))
void anim_r23Init(LedChainDesc_t *const lcd)
{
  assrt(lcd->diffR);

	for (uint32_t i = 0; i < lcd->diffR->size; ++i)
	{
		lcd->diffR->lDc[i].r.r = 0L;
		lcd->diffR->lDc[i].g.r = 0L;
		lcd->diffR->lDc[i].b.r = 0L;

		lcd->diffR->lDc[i].rP.r = 0L;
		lcd->diffR->lDc[i].gP.r = 0L;
		lcd->diffR->lDc[i].bP.r = 0L;

		lcd->diffR->lDc[i].itCur = 0u;
		lcd->diffR->lDc[i].itMax = 0u;
	}
}

void anim_r23DeInit(LedChainDesc_t *const lcd)
{
//	if(diff.lDc)
//	{
//		free(diff.lDc);
//		diff.lDc = NULL;
//	}
}

void anim_random1(LedChainDesc_t *const lcd)
{
	for (uint32_t i = 0; i < lcd->lRawNew->ledCount; ++i)
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
#endif
static void __attribute__ ((noinline)) anim_Diff(LedChainDesc_t *const lcd, uint32_t i, bool isR3)
{
	rand_u r;
	LedLogic_t l;
  assrt(lcd->diffR);
  assrt(i<lcd->diffR->size);
	HAL_RNG_GenerateRandomNumber(&hrng, &r.u32);
	led_getLedColor(lcd, i, &l);

	fpa_t div = {.r = 0};

	if (isR3)
	{
		lcd->diffR->lDc[i].itCur = 0u;
		lcd->diffR->lDc[i].itMax = r.d;

		if (lcd->diffR->lDc[i].itMax == 0u)
		{
			++lcd->diffR->lDc[i].itMax;
		}

		div.i = lcd->diffR->lDc[i].itMax;

	}
	else
	{
		div.i = cycleMin_r23;
	}

	lcd->diffR->lDc[i].r.i = l.r;
	lcd->diffR->lDc[i].g.i = l.g;
	lcd->diffR->lDc[i].b.i = l.b;

	lcd->diffR->lDc[i].rP = FPA_IntDivFpa(r.a - l.r, div);
	lcd->diffR->lDc[i].gP = FPA_IntDivFpa(r.b - l.g, div);
	lcd->diffR->lDc[i].bP = FPA_IntDivFpa(r.c - l.b, div);
}

static void __attribute__ ((noinline)) anim_render(LedChainDesc_t *const lcd, uint32_t i)
{
  assrt(lcd->diffR);
  assrt(i<lcd->diffR->size);
	lcd->diffR->lDc[i].r.r += lcd->diffR->lDc[i].rP.r;
	lcd->diffR->lDc[i].g.r += lcd->diffR->lDc[i].gP.r;
	lcd->diffR->lDc[i].b.r += lcd->diffR->lDc[i].bP.r;

	int32_t rOut = lcd->diffR->lDc[i].r.i;
	int32_t gOut = lcd->diffR->lDc[i].g.i;
	int32_t bOut = lcd->diffR->lDc[i].b.i;

	assrt(rOut <= UINT8_MAX);
	assrt(gOut <= UINT8_MAX);
	assrt(bOut <= UINT8_MAX);
	assrt(rOut >= 0L);
	assrt(gOut >= 0L);
	assrt(bOut >= 0L);

	led_setLedToColor(lcd, i, (uint8_t) rOut, (uint8_t) gOut, (uint8_t) bOut);
}
#if !(defined(STM32F103xB))
static void anim_r2Diff(LedChainDesc_t *const lcd)
{
	for (uint32_t i = 0; i < lcd->lRawNew->ledCount; ++i)
	{
		anim_Diff(lcd, i, false);
	}
}

static void anim_r2CalcAndSet(LedChainDesc_t *const lcd)
{
	for (uint32_t i = 0; i < lcd->lRawNew->ledCount; ++i)
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
#endif
void anim_random3(mAnim_t *ctx)
{
  assrt(ctx->lcd_ctx->diffR);
	for (uint32_t i = 0; i < ctx->lcd_ctx->lRawNew->ledCount; ++i)
	{
    assrt(i<ctx->lcd_ctx->diffR->size);

		if (ctx->lcd_ctx->diffR->lDc[i].itCur == ctx->lcd_ctx->diffR->lDc[i].itMax)
		{
			anim_Diff(ctx->lcd_ctx, i, true);
		}

		anim_render(ctx->lcd_ctx, i);
		++ctx->lcd_ctx->diffR->lDc[i].itCur;
	}
}
void anim_setAllLedsToUniColors(mAnim_t *ctx)
{
  led_setAllLedsToColor(ctx->lcd_ctx, 20u, 0u, 20u);
}
/** @} */
