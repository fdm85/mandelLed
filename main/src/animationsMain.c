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

extern uint32_t HAL_GetTick(void);

static anim_mode_e currMode = anim_cR2;
static uint8_t brightness = 255u;
static rider_t rider1;
static rider_t rider2;
static rider_t rider3;

rider_t *rA[] =
{ &rider1, &rider2, &rider3, NULL };

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

typedef enum {
	init,
	increment,
	done,
} puState_t;
static puState_t puS = init;
#define step	(10u)
static void powerUp(void)
{
	static uint32_t last;
	static uint8_t i = 1u;
	switch (puS) {
		case init:
			last = HAL_GetTick();
			led_setAllLedsToColor(i,i,i);
			puS = increment;
			break;
		case increment:
			if( (HAL_GetTick() - last) > 20uL)
			{
				last = HAL_GetTick();
				if(i < (UINT8_MAX - step))
				{
					i = (uint8_t)(i + step);
				}
				else
				{
					i = UINT8_MAX;
					puS = done;
				}
				led_setAllLedsToColor(i,i,i);
			}

			break;

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
	if(puS != done)
	{
		powerUp();
		return;
	}
	switch (currMode)
	{
	case anim_powerUp:
		anim_circularRun1(brightness);
		break;
	case anim_cR1:
		anim_circularRun1(brightness);
		break;
	case anim_cR2:
		riderBlanker(&rider1);
		riderFiller(&rider1);
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
	default:
		assrt(false);
		break;
	}
}
