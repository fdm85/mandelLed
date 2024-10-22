/**
 * @file      ledData.c
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
 * @brief Low level implementation of driver for leds
 * @ingroup AccessAbstraction
 * @{
 */

#include "ledData.h"
#include <stdint.h>
#include "assrt.h"
#include "leds.h"
#include "tim.h"

//#define LOC_INLINE	inline
#define LOC_INL_DBG	__attribute__ ((noinline))

uint32_t getLedCount(LedChainDesc_t* lcd)
{
	return lcd->lRawNew->ledCount;
}

/// raw bits are stored MSB first, order is green, red, blue
static void led_convertLed(const LedLogic_t *l, LedRaw *r)
{
	for (uint8_t i = 0; i < 8u; ++i)
	{
		r->g[i] = (l->g & (0x80u >> i)) ? lRawOn : lRawOff;
		r->r[i] = (l->r & (0x80u >> i)) ? lRawOn : lRawOff;
		r->b[i] = (l->b & (0x80u >> i)) ? lRawOn : lRawOff;
	}
}
/// raw bits are stored MSB first, order is green, red, blue
static void led_convertLedToZero(LedRaw *r)
{
	for (uint8_t i = 0; i < 8u; ++i)
	{
		r->g[i] = 0u;
		r->r[i] = 0u;
		r->b[i] = 0u;
	}
}

void led_LedLogicInit(LedChainDesc_t* lcd)
{
	led_setAllLedsToUniColors(lcd, 0u);
}

/** @brief Set brightness truncation
 *  @param lcd context to work on
 *  @param mult Multiplier
 *  @param div Divider
 */
void led_setBrightnessTruncation(LedChainDesc_t* lcd, uint32_t mult, uint32_t div)
{
	lcd->btMult = mult;
	lcd->btDiv = div;
}

/** @brief Set a specific led's colors
 *  @param lcd led to work on
 *  @param r red color
 *  @param g green color
 *  @param b blue color
 *  @param mult multiplier
 *  @param div divider
 */
static void led_setLedColors(LedLogic_t *led, uint8_t r, uint8_t g, uint8_t b, uint32_t mult, uint32_t div)
{
	uint32_t rOut = (uint32_t) (r * mult) / div;
	uint32_t gOut = (uint32_t) (g * mult) / div;
	uint32_t bOut = (uint32_t) (b * mult) / div;

	assrt(rOut <= UINT8_MAX);
	assrt(gOut <= UINT8_MAX);
	assrt(bOut <= UINT8_MAX);
	led->r = (uint8_t) rOut;
	led->g = (uint8_t) gOut;
	led->b = (uint8_t) bOut;
}

/** @brief Set a specific led's colors
 *  @param lcd strip context to work on
 *  @param i index of led to set
 *  @param r red color
 *  @param g green color
 *  @param b blue color
 */
void led_setLedToColor(LedChainDesc_t* lcd, uint32_t i, uint8_t r, uint8_t g, uint8_t b)
{
	if(i > lcd->lRawNew->ledCount)
		assrt(false);
	led_setLedColors(&lcd->lLogic[i], r, g, b, lcd->btMult, lcd->btDiv);
}

/** @brief Get a specific led's colors
 *  @param lcd strip context to work on
 *  @param i index of led to set
 *  @param l were to write the colors to
 */
void led_getLedColor(LedChainDesc_t *const lcd, uint32_t i, LedLogic_t *l)
{
	assrt(l);
	if(i > lcd->lRawNew->ledCount)
		assrt(false);
	l->b = lcd->lLogic[i].b;
	l->g = lcd->lLogic[i].g;
	l->r = lcd->lLogic[i].r;
}

/** @brief Set all leds of the strip to the same color
 *  @param lcd strip context to work on
 *  @param r red color
 *  @param g green color
 *  @param b blue color
 */
void led_setAllLedsToColor(LedChainDesc_t* lcd, uint8_t r, uint8_t g, uint8_t b)
{
	for (uint16_t i = 0; i < lcd->lRawNew->ledCount; ++i)
	{
		led_setLedColors(&lcd->lLogic[i], r, g, b, lcd->btMult, lcd->btDiv);
	}
}

/** @brief Set leds [s,e] of the strip to the same color
 *  @param lcd strip context to work on
 *  @param r red color
 *  @param g green color
 *  @param b blue color
 *  @param s start
 *  @param e end
 */
void led_setFromToLedsToColor(LedChainDesc_t* lcd, uint8_t r, uint8_t g, uint8_t b, uint32_t s, uint32_t e)
{
	for (uint32_t i = s; i < e; ++i)
	{
		led_setLedColors(&lcd->lLogic[i], r, g, b, lcd->btMult, lcd->btDiv);
	}
}

/** @brief Set all leds of the strip to the same color (uni color)
 *  @param lcd strip context to work on
 *  @param brightness brightness set val
 */
void led_setAllLedsToUniColors(LedChainDesc_t* lcd, uint8_t brightness)
{
	for (uint16_t i = 0; i < lcd->lRawNew->ledCount; ++i)
	{
		led_setLedColors(&lcd->lLogic[i], brightness, brightness, brightness, lcd->btMult, lcd->btDiv);
	}
}


/** @brief Convert/Paste logic rgb colors to the raw data out field
 *  @param lcd strip context to work on
 */
void led_pasteData(LedChainDesc_t* lcd)
{
	for (uint16_t i = 0; i < lcd->lRawNew->ledCount; ++i)
	{
		led_convertLed(&lcd->lLogic[i], &lcd->lRawNew->lRaw[i]);
	}
}

/** @brief Trigger data transmission
 *  @param lcd strip context to work on
 */
//static LOC_INL_DBG void led_startTransmitData(LedChainDesc_t* lcd)
LOC_INL_DBG void led_startTransmitData(LedChainDesc_t* lcd)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Start_DMA(lcd->timer, lcd->timChannel, &lcd->lRawNew->lRaw[0].g[0], (lcd->lRawNew->rawTxCount));
	assrt(result == HAL_OK);
	(void) result;
}
/** @brief Trigger data transmission
 *  @param lcd strip context to work on
 */
static LOC_INL_DBG void led_stopTransmitData(LedChainDesc_t* lcd)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Stop_DMA(lcd->timer, lcd->timChannel);
	assrt(result == HAL_OK);
	lcd->lRawNew->rS = e_done;
	(void) result;
}

#define inFrame 6uL
#define outFrame inFrame
static const LedLogic_t cLed = {.g = 10u}; /*!< converter led set color */
static LOC_INL_DBG void fillFade(LedChainDesc_t* lcd)
{
  /// assuming dma buffer is at least twice as big as (inFrame + cLed)
	for (lcd->lRawNew->iS = 0uL; lcd->lRawNew->iS < inFrame ; ++lcd->lRawNew->iS, ++lcd->lRawNew->iD) {
	  led_convertLedToZero(&lcd->lRawNew->lRaw[lcd->lRawNew->iD]);
	}
	led_convertLed(&cLed, &lcd->lRawNew->lRaw[lcd->lRawNew->iD]);
	++lcd->lRawNew->iD;

	/// add first segment of real data here to simplify implementation of half cycle filler
	if(lcd->lRawNew->rS == e_Precursor)
    for (lcd->lRawNew->iS = 0uL;
        (lcd->lRawNew->iS < lcd->lRawNew->ledCount) && (lcd->lRawNew->iD < lcd->lRawNew->rawCount);
         ++lcd->lRawNew->iS, ++lcd->lRawNew->iD) {
      led_convertLed(&lcd->lLogic[lcd->lRawNew->iS], &lcd->lRawNew->lRaw[lcd->lRawNew->iD]);
    }
}

static LOC_INL_DBG void fillRealData(LedChainDesc_t* lcd)
{
	const uint32_t iMax = (lcd->lRawNew->rawCount / 2u);
	const uint32_t iOffset = (lcd->lRawNew->dS == e_SecondHalf) ? (lcd->lRawNew->rawCount / 2u) : 0;
  for (lcd->lRawNew->iD = 0uL;
      (lcd->lRawNew->iD < iMax) && (lcd->lRawNew->iS < lcd->lRawNew->ledCount);
       ++lcd->lRawNew->iS, ++lcd->lRawNew->iD) {
    led_convertLed(&lcd->lLogic[lcd->lRawNew->iS], &lcd->lRawNew->lRaw[lcd->lRawNew->iD + iOffset]);
  }
}
volatile eDmaRawFill list[30] = {e_Inv};
volatile dmaState_t list2[30] = {e_Inv2};
static uint8_t idx = 0u;
void led_txRaw(LedChainDesc_t* lcd)
{
  list[idx] = lcd->lRawNew->rS;
  list2[idx] = lcd->lRawNew->dS;

	/// is it possible to change dma size during active run?
	switch (lcd->lRawNew->rS) {
		case e_Precursor:
			fillFade(lcd);
			led_startTransmitData(lcd);
			lcd->lRawNew->rS = e_realData;
			break;
		case e_realData:
			fillRealData(lcd);
			if(lcd->lRawNew->iS == lcd->lRawNew->ledCount)
				lcd->lRawNew->rS = e_Tail_1;
			break;
		case e_Tail_1:
			fillFade(lcd);
			lcd->lRawNew->rS = e_Tail_2;
			break;
		case e_Tail_2:
			fillFade(lcd);
			lcd->lRawNew->rS = e_done;
			break;
		case e_done:
				led_stopTransmitData(lcd);
				idx = 0u;
			break;
		default:
			break;
	}
  if(++idx >= 30)
    idx = 0u;
}

/** @}*/
