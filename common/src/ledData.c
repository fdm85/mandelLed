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

#include "tim.h"


uint32_t getLedCount(LedChainDesc_t* lcd)
{
	return lcd->lRaw->ledCount;
}

/// raw bits are stored MSB first, order is green, red, blue
static void led_convertLed(LedChainDesc_t* lcd, LedLogic_t *l, LedRaw *r)
{
	for (uint8_t i = 0; i < 8u; ++i)
	{
		r->g[i] = (l->g & (0x80u >> i)) ? lcd->rawOn : lcd->rawOff;
		r->r[i] = (l->r & (0x80u >> i)) ? lcd->rawOn : lcd->rawOff;
		r->b[i] = (l->b & (0x80u >> i)) ? lcd->rawOn : lcd->rawOff;
	}
}

void led_initDataRaw(LedChainDesc_t* lcd)
{
	lcd->lRaw->rI[0] = 0uL;
	lcd->lRaw->rO[0] = 0uL;
	LedLogic_t init = {12u, 0u, 0u};
	led_convertLed(lcd, &init, lcd->lRaw->lConverterLed);
	for (uint16_t i = 1; i < resLength; ++i)
	{
		lcd->lRaw->rI[i] = 0;
		lcd->lRaw->rO[i] = 0;
	}
	led_setAllLedsToUniColors(lcd, 0u);
	led_pasteData(lcd);
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
	if(i > lcd->lRaw->ledCount)
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
	for (uint16_t i = 0; i < lcd->lRaw->ledCount; ++i)
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
	for (uint16_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{
		led_setLedColors(&lcd->lLogic[i], brightness, brightness, brightness, lcd->btMult, lcd->btDiv);
	}
}


/** @brief Convert/Paste logic rgb colors to the raw data out field
 *  @param lcd strip context to work on
 */
void led_pasteData(LedChainDesc_t* lcd)
{
	for (uint16_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{
		led_convertLed(lcd, &lcd->lLogic[i], &lcd->lRaw->lRaw[i]);
	}
}

/** @brief Trigger data transmission
 *  @param lcd strip context to work on
 */
static inline void led_startTransmitData(LedChainDesc_t* lcd)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Start_DMA(lcd->timer, lcd->timChannel, &lcd->lRaw->rI[0], lcd->lRaw->txCountInUi32);
	assrt(result == HAL_OK);
	(void) result;
}
/** @brief Trigger data transmission
 *  @param lcd strip context to work on
 */
static inline void led_stopTransmitData(LedChainDesc_t* lcd)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Stop_DMA(lcd->timer, lcd->timChannel);
	assrt(result == HAL_OK);
	lcd->lRawNew->dS = e_done;
	(void) result;
}

static inline void fillFade(LedChainDesc_t* lcd)
{
	const uint32_t iMax = lcd->lRawNew->rawCount * 24uL;
	for (uint32_t i = 0; i < iMax; ++i) {
		((uint32_t*)lcd->lRawNew->lRaw)[i] = 0uL;
	}
}

static inline void fillRealData(LedChainDesc_t* lcd)
{
	const uint32_t iMax = (lcd->lRawNew->rS == e_Init) ? lcd->lRawNew->rawCount : (lcd->lRawNew->rawCount / 2u);
	const uint32_t iOffset = (lcd->lRawNew->rS == e_SecondHalf) ? (lcd->lRawNew->rawCount / 2u) : 0;
	for (uint32_t i = 0; (i < iMax) && (lcd->lRawNew->i < lcd->lRawNew->ledCount); ++i, ++lcd->lRawNew->i) {
		led_convertLed(lcd, &lcd->lLogic[lcd->lRawNew->i], &lcd->lRawNew->lRaw[iOffset + i]);
	}
}

void led_txRaw(LedChainDesc_t* lcd)
{
	/// is it possible to change dma size during active run?
	switch (lcd->lRawNew->dS) {
		case e_fadeIn:
			fillFade(lcd);
			led_startTransmitData(lcd);
			lcd->lRawNew->dS = e_realData;
			break;
		case e_realData:
			fillRealData(lcd);
			lcd->lRawNew->dS = e_realData;
			break;
		case e_fadeOut:
			fillFade(lcd);
			lcd->lRawNew->dS = e_done;
			break;
		case e_done:
			if(lcd->lRawNew->rS == e_SecondHalf)
			{
				led_stopTransmitData(lcd);

			}
			break;
		default:
			break;
	}
}

/** @}*/
