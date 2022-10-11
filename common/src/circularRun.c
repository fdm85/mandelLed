/**
 * @file      circularRun.c
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
 * @brief Circular run implementation
 * @defgroup Circular_Run Animations that feature a circular(repeating) manner
 * @ingroup Animations
 *
 * @{
 */

#include "animations.h"
#include "ledData.h"
#include "fpa.h"

#define cycle 3u
#define cycleShort 0u
//static uint8_t cycleCount = cycle;
static uint8_t cycleCount = cycleShort;

void anim_setCirc(bool shrt)
{
	cycleCount = (shrt) ? cycleShort : cycle;
}

void anim_circularRun1(LedChainDesc_t* lcd, uint8_t brightness)
{
	static uint32_t index = 0u;
	static int32_t sign = 1;
	if (!cycleCount)
	{

		led_setAllLedsToUniColors(lcd, 0);
		led_setLedToColor(lcd, index, 0u, 0u, brightness);
		led_setLedToColor(lcd, (index + 1u), 0u, brightness, 0u);
		led_setLedToColor(lcd, (index + 2u), brightness, 0u, 0u);
		led_setLedToColor(lcd, index + 20u, 0u, 0u, brightness);
		led_setLedToColor(lcd, (index + 21u), 0u, brightness, 0u);
		led_setLedToColor(lcd, (index + 22u), brightness, 0u, 0u);
		led_setLedToColor(lcd, index + 40u, 0u, 0u, brightness);
		led_setLedToColor(lcd, (index + 41u), 0u, brightness, 0u);
		led_setLedToColor(lcd, (index + 42u), brightness, 0u, 0u);

		index = (uint32_t)((int32_t)index + sign);
		if((index + 43 == lcd->lRaw->ledCount) || (index == 0))
			sign *= -1;
		cycleCount = cycleShort;
	}
	else
	{
		--cycleCount;
	}
}

void anim_initRedRider(LedChainDesc_t *const lcd, rider_t *arg)
{
	arg->pos = 0uL;
	arg->posMin = 0uL;
	arg->posMax = getLedCount(lcd);
	arg->c.r = 255u;
	arg->c.g = 0u;
	arg->c.b = 0u;
	arg->step.r = _FPA_(2);
	arg->length = 8u;
	arg->blanks = 4u;
	arg->posIq.r = _FPA_(arg->pos);
}

void anim_initRedRider2(LedChainDesc_t *const lcd, rider_t *arg)
{
	arg->posMin = 0uL;
	arg->posMax = getLedCount(lcd);
	arg->pos = arg->posMax / 2uL;
	arg->c.r = 100u;
	arg->c.g = 200u;
	arg->c.b = 0u;
	arg->step.r = _FPA_(3.25);
	arg->length = 6u;
	arg->blanks = 4u;
	arg->posIq.r = _FPA_(arg->pos);
}

void anim_initRedRider3(LedChainDesc_t *const lcd, rider_t *arg)
{
	arg->posMin = 0uL;
	arg->posMax = getLedCount(lcd);
	arg->c.r = 0u;
	arg->c.g = 0u;
	arg->c.b = 255u;
	arg->step.r = _FPA_(-1.5);
	arg->length = 8u;
	arg->pos = arg->posMax - arg->length;
	arg->blanks = 4;
	arg->posIq.r = _FPA_(arg->pos);
}

void anim_initPuRide(LedChainDesc_t *const lcd, rider_t *arg, uint8_t r, uint8_t g, uint8_t b, uint8_t dir)
{
	arg->posMin = 0uL;
	arg->posMax = getLedCount(lcd);
	arg->c.r = r;
	arg->c.g = g;
	arg->c.b = b;
	arg->step.r = dir ? _FPA_(-15) : _FPA_(15);
	arg->length = 8u;
	arg->pos = getLedCount(lcd) / 2uL;
	arg->blanks = 4;
	arg->posIq.r = _FPA_(arg->pos);
	arg->iteration = 1u;
}

void riderBlanker(LedChainDesc_t *const lcd, rider_t *arg)
{
	if (!((arg->pos > arg->posMin) && (arg->pos < arg->posMax)))
		return;
	for (uint8_t i = 1; i <= arg->blanks; ++i)
	{

		if ((int32_t) arg->pos - i >= (int32_t) arg->posMin)
			led_setLedToColor(lcd, arg->pos - i, 0u, 0u, 0u);
	}
	for (uint8_t i = 0; i < arg->blanks; ++i)
	{

		if (arg->pos + (uint32_t) (arg->length + i) <= arg->posMax)
			led_setLedToColor(lcd, arg->pos + (uint32_t) (arg->length + i), 0u, 0u,
					0u);
	}
}

void riderFiller(LedChainDesc_t *const lcd, rider_t *arg)
{
	for (uint8_t i = 0; i < arg->length; ++i)
	{
		led_setLedToColor(lcd, (arg->pos + i), arg->c.r, arg->c.g, arg->c.b);
	}
	if ((arg->pos >= (arg->posMax - arg->length)) && (arg->step.i > 0))
	{
		arg->step.r *= -1;
	}
	if ((arg->pos <= (arg->posMin + arg->length)) && (arg->step.i < 0))
	{
		arg->step.r *= -1;
	}

	arg->posIq.r += arg->step.r;

	arg->pos = (uint32_t)arg->posIq.i;

}

void rideOnceFiller(LedChainDesc_t *const lcd, rider_t *arg)
{
	if (!arg->length)
		return;
	if (!((arg->pos > arg->posMin) && (arg->pos < arg->posMax)))
	{
		arg->length = 0;
		return;
	}
	arg->iteration = (uint8_t) ((arg->iteration << 1u) + 1);

	if ((arg->pos >= (arg->posMax - arg->length)) && (arg->step.i > 0))
	{
		arg->length = 0;
	}
	if ((arg->pos <= (arg->posMin + arg->length)) && (arg->step.i < 0))
	{
		arg->length = 0;
	}

	for (uint8_t i = 0; i < arg->length; ++i)
	{
		led_setLedToColor(lcd, (arg->pos + i), arg->iteration & arg->c.r,
				arg->iteration & arg->c.g, arg->iteration & arg->c.b);
	}

	if (arg->length)
	{
		arg->posIq.r += arg->step.r;
		arg->pos = (uint32_t)arg->posIq.i;
	}
}
/** @}*/
