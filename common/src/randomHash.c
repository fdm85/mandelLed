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
#endif
#ifdef STM32L476xx
#include "rng.h"
#endif

/** @brief led color transition descriptor
 * @details compound with all needed details to run random diff color animation
*/
typedef struct Led_diffColor{
    fpa_t g; /*!< green diff per iteration */
    fpa_t r; /*!< red diff per iteration */
    fpa_t b; /*!< blue diff per iteration */
    fpa_t gP; /*!< green last set val */
    fpa_t rP; /*!< red last set val */
    fpa_t bP; /*!< blue last set val */
    uint16_t itCur; /*!< iteration counter */
    uint16_t itMax; /*!< target iteration count */
}Led_progColor_t;

/** @brief diff runner context
 * @details adapter to couple diff animation array to a strip */
typedef struct diffRunnerCtx_tag
{
    Led_progColor_t * lDc; /*!< reference to strip to run on */
    uint32_t size; /*!< size/length of animation on the strip */
    // todo add start point
}diffRunnerCtx_t;

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

// todo fix this, create factory macro to allocate
Led_progColor_t __attribute__((section (".ccmram"))) prog_r23[619];
diffRunnerCtx_t diff = {.lDc = &prog_r23[0], .size = 619uL};
static uint16_t cycleMin_r23 = 100u;
static uint16_t it_r2 = 100u;

void anim_r23Init(LedChainDesc_t *const lcd)
{
//	if(!diff.lDc)
//		diff.lDc = malloc(sizeof(Led_progColor_t) * diff.size);

	for (uint32_t i = 0; i < diff.size; ++i)
	{
		diff.lDc[i].r.r = 0L;
		diff.lDc[i].g.r = 0L;
		diff.lDc[i].b.r = 0L;

		diff.lDc[i].rP.r = 0L;
		diff.lDc[i].gP.r = 0L;
		diff.lDc[i].bP.r = 0L;

		diff.lDc[i].itCur = 0u;
		diff.lDc[i].itMax = 0u;
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

	fpa_t div = {.r = 0};

	if (isR3)
	{
		diff.lDc[i].itCur = 0u;
		diff.lDc[i].itMax = r.d;

		if (diff.lDc[i].itMax == 0u)
		{
			++diff.lDc[i].itMax;
		}

		div.i = diff.lDc[i].itMax;

	}
	else
	{
		div.i = cycleMin_r23;
	}

	diff.lDc[i].r.i = l.r;
	diff.lDc[i].g.i = l.g;
	diff.lDc[i].b.i = l.b;

	diff.lDc[i].rP = FPA_IntDivFpa(r.a - l.r, div);
	diff.lDc[i].gP = FPA_IntDivFpa(r.b - l.g, div);
	diff.lDc[i].bP = FPA_IntDivFpa(r.c - l.b, div);
}

static void anim_render(LedChainDesc_t *const lcd, uint32_t i)
{

	diff.lDc[i].r.r += diff.lDc[i].rP.r;
	diff.lDc[i].g.r += diff.lDc[i].gP.r;
	diff.lDc[i].b.r += diff.lDc[i].bP.r;

	int32_t rOut = diff.lDc[i].r.i;
	int32_t gOut = diff.lDc[i].g.i;
	int32_t bOut = diff.lDc[i].b.i;

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

/** @brief rainbow effect style
 *  @details Each single led receives a random rgb set value and cycle-count in which a transition from current rgb to set-value is performed.
 *
 *  @param lcd led-chain context to use
 */
void anim_random3(LedChainDesc_t *const lcd)
{
   //! do for all leds in the strip
	for (uint32_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{
	   //! transition complete?
		if (diff.lDc[i].itCur == diff.lDc[i].itMax)
		{
		   //! calculate new cycle
			anim_Diff(lcd, i, true);
		}
		//! perform a step of the transition
		anim_render(lcd, i);
		++diff.lDc[i].itCur;
	}
}
/** @} */
