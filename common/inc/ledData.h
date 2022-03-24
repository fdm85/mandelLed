/**
 * @file      ledData.h
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
 * @brief Low level driver for leds
 * @defgroup Led_Data Low level led data handling
 * @ingroup Animations
 */

#ifndef LEDDATA_H_
#define LEDDATA_H_

#include <stdint.h>
#include "lm.h"
#include "fpa.h"
#include "tim.h"

/** @brief logical led container */
typedef struct LedLogic_tag{
	uint8_t g; /*!< */
	uint8_t r; /*!< */
	uint8_t b; /*!< */
}LedLogic_t;

/** @brief led color transition descriptor */
typedef struct Led_diffColor{
	fpa_t g; /*!< */
	fpa_t r; /*!< */
	fpa_t b; /*!< */
	fpa_t gP; /*!< */
	fpa_t rP; /*!< */
	fpa_t bP; /*!< */
	uint16_t itCur; /*!< */
	uint16_t itMax; /*!< */
}Led_progColor_t;

/** @brief diff runner context */
typedef struct diffRunnerCtx_tag
{
	Led_progColor_t * lDc; /*!< */
	uint32_t size; /*!< */
}diffRunnerCtx_t;

/** @brief raw led data for DMA to timer transfer */
typedef struct LedRaw
{
	uint32_t g[8]; /*!< */
	uint32_t r[8]; /*!< */
	uint32_t b[8]; /*!< */
} LedRaw;

/** @brief raw content context of a strip */
typedef struct lRawCont_tag
{
	uint32_t* rI; /*!< */
	uint32_t* rO; /*!< */
	LedRaw* lConverterLed; /*!< */
	LedRaw* lRaw; /*!< */
	uint32_t ledCount; /*!< */
	uint16_t txCountInUi32; /*!< */
	uint16_t padding; /*!< */
}lRawCont_t;

/** @defgroup MemoryAbstraction Memory Abstraction
 *  @ingroup Led_Data*/

/** @defgroup AccessAbstraction LED access abstraction
 *  @ingroup Led_Data*/

/** @brief defines time(size) of in/out low level phase
 *  @ingroup MemoryAbstraction */
#define resLength 41u

/** @brief factory macro to create the raw data container
 *  @ingroup MemoryAbstraction */
#define lRawContainer(name, ledCnt) \
	static struct \
	{ \
		uint32_t rI[resLength]; \
		LedRaw converterLed[1]; \
		LedRaw ledRaw[ledCnt]; \
		uint32_t rO[resLength]; \
	} lRawContainer_##name; \
	const lRawCont_t lRawCont_##name = { \
			.rI = &lRawContainer_##name.rI[0], \
			.rO = &lRawContainer_##name.rO[0], \
			.lConverterLed = &lRawContainer_##name.converterLed[0], \
			.lRaw = &lRawContainer_##name.ledRaw[0], \
			.ledCount = ledCnt, \
			.txCountInUi32 = (sizeof(lRawContainer_##name)/sizeof(uint32_t)), \
	}
/** @brief factory macro to create the logical LED data container
 *  @ingroup MemoryAbstraction */
#define lLogicContainer(name, ledCnt)\
	static LedLogic_t CCRAM_PLACING ledsLog_##name[ledCnt]

/** @brief factory macro to tie a logic LED strip to a timer PWM output channel
 *  @ingroup MemoryAbstraction */
#define lChainDesc(name, timerN, tChan, rOn, rOff) \
	LedChainDesc_t lcd_##name = { \
			.lLogic = &ledsLog_##name[0], \
			.lRaw = &lRawCont_##name, \
			.timer = &timerN, \
			.timChannel = tChan, \
			.rawOn = rOn, \
			.rawOff = rOff, \
	}

/** @brief led chain descriptor, one for each led tube/chain
 *  @ingroup Led_Data */
typedef struct LedChainDesc_tag
{
	LedLogic_t* lLogic; /*!< pointer to led container, i.e. the RGB values of each single LED in a strip */
	const lRawCont_t *const lRaw; /*!< pointer to IO-out raw data of the strip */
	TIM_HandleTypeDef* timer; /*!< pointer to the timer instance responsible for the data output*/
	uint32_t timChannel; /*!< output channel of the timer (as a timer peripheral my have multiple channels) */
	uint32_t rawOn; /*!< pwm setval to produce a set bit*/
	uint32_t rawOff; /*!< pwm setval to produce a reset bit*/
	uint32_t btMult; /*!< brightness truncation multiplier */
	uint32_t btDiv; /*!< brightness truncation divider */
} LedChainDesc_t;

void led_initDataRaw(LedChainDesc_t* lcd);
void led_setAllLedsToUniColors(LedChainDesc_t* lcd, uint8_t brightness);
void led_setLedToColor(LedChainDesc_t* lcd, uint32_t i, uint8_t r, uint8_t g, uint8_t b);
void led_getLedColor(LedChainDesc_t* lcd, uint32_t i, LedLogic_t *l);
void led_pasteData(LedChainDesc_t* lcd);
void led_transmitData(LedChainDesc_t* lcd);
void led_setBrightnessTruncation(LedChainDesc_t* lcd, uint32_t mult, uint32_t div);
void led_setAllLedsToColor(LedChainDesc_t* lcd, uint8_t r, uint8_t g, uint8_t b);
uint32_t getLedCount(LedChainDesc_t* lcd);

#endif /* LEDDATA_H_ */
