/**
 * @file      animationsMain.c
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
 * @brief Main animations implementation
 * @ingroup Animations
 * @{
 */

#include "animations.h"
#include "assrt.h"
#include "ledData.h"
#include "physic.h"
#include "matrix.h"
#include "msgeq7.h"
//static uint8_t brightness = 255u;
//static anim_mode_e currMode = anim_cR2;
#if USE_RIDERS
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

void anim_setMode(LedChainDesc_t *const lcd, anim_mode_e set)
{
	assrt(set < anim_enumAssrt);
	currMode = set;
	if ((set == anim_rnd2) || (set == anim_rnd3))
	{
		anim_r23Init(lcd);
	}
	else if ((set == anim_cR2) || (set == anim_layers))
	{
		anim_r23Init(lcd);
		anim_initRedRider(lcd, &rider1);
		anim_initRedRider2(lcd, &rider2);
		anim_initRedRider3(lcd, &rider3);
	}
	else
	{
		anim_r23DeInit(lcd);
	}
}

void anim_nextMode(LedChainDesc_t *const lcd)
{
	anim_mode_e set = currMode;
	++set;

	if (set == anim_enumAssrt)
	{
		set = anim_min;
	}

	anim_setMode(lcd, set);
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
static void powerUp(mAnim_t* ctx)
{
	static uint8_t breaker = 0u;
	static uint32_t ledCount = 0uL;

	switch (ctx->puState)
	{
	case init:
		ledCount = getLedCount(ctx->lcd_ctx);
		led_setAllLedsToColor(ctx->lcd_ctx, 0, 0, 0);
		anim_initPuRide(ctx->lcd_ctx, &rPu01, 0u, 0u, 255u, 0u);
		anim_initPuRide(ctx->lcd_ctx, &rPu11, 0u, 0u, 255u, 1u);
		anim_initPuRide(ctx->lcd_ctx, &rPu02, 0u, 255u, 0u, 0u);
		anim_initPuRide(ctx->lcd_ctx, &rPu12, 0u, 255u, 0u, 1u);
		anim_initPuRide(ctx->lcd_ctx, &rPu03, 255u, 0u, 0u, 0u);
		anim_initPuRide(ctx->lcd_ctx, &rPu13, 255u, 0u, 0u, 1u);
		anim_initPuRide(ctx->lcd_ctx, &rPu04, 255u, 255u, 255u, 0u);
		anim_initPuRide(ctx->lcd_ctx, &rPu14, 255u, 255u, 255u, 1u);
		ctx->puState = blueSpawn;
		break;
	case blueSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(ctx->lcd_ctx, rPu1[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu1[i]);
		}
		if (rPu01.iteration > 32u)
			ctx->puState = greenSpawn;
		break;
	case greenSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(ctx->lcd_ctx, rPu1[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu1[i]);
			riderBlanker(ctx->lcd_ctx, rPu2[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu2[i]);
		}
		if (rPu02.iteration > 32u)
			ctx->puState = redSpawn;
		break;
	case redSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(ctx->lcd_ctx, rPu1[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu1[i]);
			riderBlanker(ctx->lcd_ctx, rPu2[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu2[i]);
			riderBlanker(ctx->lcd_ctx, rPu3[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu3[i]);
		}
		if (rPu03.iteration > 32u)
			ctx->puState = whiteSpawn;
		break;
	case whiteSpawn:
		for (uint8_t i = 0; i < 2u; ++i)
		{
			riderBlanker(ctx->lcd_ctx, rPu1[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu1[i]);
			riderBlanker(ctx->lcd_ctx, rPu2[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu2[i]);
			riderBlanker(ctx->lcd_ctx, rPu3[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu3[i]);
			rideOnceFiller(ctx->lcd_ctx, rPu4[i]);
		}
		if (!rPu04.length)
			ctx->puState = fullWhite;
		break;
	case fullWhite:
	{
		LedLogic_t l;
		++breaker;
		for (uint32_t i = 0; i < ledCount; ++i) {
			led_getLedColor(ctx->lcd_ctx, i, &l);

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

			led_setLedToColor(ctx->lcd_ctx, i, l.r, l.g, l.b);
		}
		if(breaker > 12)
			ctx->puState = done;
		break;
	}

	case done:
		return;
	default:
		__BKPT(0);
		break;
	}
}

static void layers(LedChainDesc_t *const lcd)
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
#endif
void anim_CyclicCall(mAnim_t* ctx)
{
#if USE_RIDERS
	if (ctx->puState != done)
	{
		powerUp(ctx);
		return;
	}
#endif
	switch (ctx->AnimMode)
	{
//	case anim_cR1:
//		anim_circularRun1(ctx->lcd_ctx, brightness);
//		break;
//	case anim_cR2:
//		riderBlanker(ctx->lcd_ctx, &rider1);
//		riderFiller(ctx->lcd_ctx, &rider1);
//		break;
//	case anim_rnd1:
//		anim_random1(ctx->lcd_ctx);
//		break;
//	case anim_rnd2:
//		anim_random2(ctx->lcd_ctx);
//		break;
	case anim_rnd3:
		anim_random3(ctx->lcd_ctx);
		break;
#if USE_RIDERS
	case anim_white:
		led_setAllLedsToColor(ctx->lcd_ctx, brightness, brightness, brightness);
		break;
	case anim_red:
		led_setAllLedsToColor(ctx->lcd_ctx, brightness, 0, 0);
		break;
	case anim_green:
		led_setAllLedsToColor(ctx->lcd_ctx, 0, brightness, 0);
		break;
	case anim_blue:
		led_setAllLedsToColor(ctx->lcd_ctx, 0, 0, brightness);
		break;
	case anim_cycleColors:
	{
		static uint8_t z = 0u;
		switch (z++)
		{
		case 0:
			led_setAllLedsToColor(ctx->lcd_ctx, brightness, brightness, brightness);
			break;
		case 1:
			led_setAllLedsToColor(ctx->lcd_ctx, brightness, 0, 0);
			break;
		case 2:
			led_setAllLedsToColor(ctx->lcd_ctx, 0, brightness, 0);
			break;
		case 3:
		default:
			led_setAllLedsToColor(ctx->lcd_ctx, 0, 0, brightness);
			z = 0u;
			break;
		}
		break;
	}
	case anim_layers:
		layers(ctx->lcd_ctx);
		break;
#endif
	case anim_msqDrv:
		for (uint8_t i = 0; frqS[i] != NULL; ++i) {
          led_setFromToLedsToColor(ctx->lcd_ctx, frqS[i]->backGround.r, frqS[i]->backGround.g, frqS[i]->backGround.b, frqS[i]->start, frqS[i]->end);
		   for (uint8_t j = 0; frqS[i]->frqB[j] != NULL; ++j) {
		         frqS[i]->f(ctx->lcd_ctx, frqS[i]->frqB[j]);
		   }
		}
//		for (uint8_t i = 0; frqB[i] != NULL; ++i) {
//			anim_frqDrv(ctx->lcd_ctx, frqB[i]);
//		}
//		for (uint8_t i = 0; frqR[i] != NULL; ++i) {
//		   anim_frqFrvRem(ctx->lcd_ctx, frqR[i]);
//		}
		break;
	default:
		assrt(false);
		break;
	}
}
/** @}*/
