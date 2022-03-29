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
	uint8_t g; /*!< green set val */
	uint8_t r; /*!< red set val */
	uint8_t b; /*!< blue set val */
}LedLogic_t;

/** @brief led color transition descriptor
 * @details compound with all needed details to run random diff color animation
 * @ingroup Random_Anim */
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
 * @details adapter to couple diff animation array to a strip
 * @ingroup Random_Anim */
typedef struct diffRunnerCtx_tag
{
	Led_progColor_t * lDc; /*!< reference to strip to run on */
	uint32_t size; /*!< size/length of animation on the strip */
	// todo add start point
}diffRunnerCtx_t;

/** @brief raw led data for DMA to timer transfer
 * @details dedicated for pwm timer usage with 32-bit timer \n dedicated for ws2812 with color sequence 1. green 2. red 3. blue
 * \n todo port to 16-bit timer to reduce memory footprint
 * @ingroup Led_Data */
typedef struct LedRaw
{
	uint32_t g[8]; /*!< green data */
	uint32_t r[8]; /*!< red data */
	uint32_t b[8]; /*!< blue data */
} LedRaw;

/** @brief raw content context of a strip
 * @details dedicated to ws2812 leds
 * @ingroup Led_Data */
typedef struct lRawCont_tag
{
	uint32_t* rI; /*!< fade in dummy part, to create low level */
	uint32_t* rO; /*!< fade out dummy part, to create low level */
	LedRaw* lConverterLed; /*!< Converter led is used as 'cheap' level shifter, so it is the first 'real' led in the strip (will be painted in plain green) */
	LedRaw* lRaw; /*!< pointer to 'real' raw led ctx */
	uint32_t ledCount; /*!< count of 'real' leds in the strip */
	uint16_t txCountInUi32; /*!< count of 'total to transmit' raw led data packets */
	uint16_t padding; /*!< padding/reserved */
}lRawCont_t;

/** @defgroup MemoryAbstraction Memory Abstraction
 *  @ingroup Led_Data*/

/** @defgroup AccessAbstraction LED access abstraction
 *  @ingroup Led_Data*/

/** @brief defines time(size) of in/out low level phase
 *  @ingroup MemoryAbstraction */
#define resLength 41u

/** @brief factory macro to create the raw data container
 *  @details will create raw data struct needed to output a full strip led-data
 *  @ingroup MemoryAbstraction
 *  @param name Name of the instance
 *  @param ledCnt count of real leds */
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
 *  @details will create logic data struct needed to handle a strip
 *  @ingroup MemoryAbstraction
 *  @param name Name of the instance
 *  @param ledCnt count of real leds */
#define lLogicContainer(name, ledCnt)\
	static LedLogic_t CCRAM_PLACING ledsLog_##name[ledCnt]

/** @brief factory macro to tie a logic LED strip to a timer PWM output channel
 *  @param name Name of the instance
 *  @param timerN timer peripheral (used to reference)
 *  @param tChan PWM channel used to output the data
 *  @param rOn PWM setval of high bit
 *  @param rOff PWM setval of low bit
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
