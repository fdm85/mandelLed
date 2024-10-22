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
	uint16_t iTx; /*!<  */
}lRawCont_t;

typedef enum
{
	e_fadeIn,
	e_FirstHalf, /*!< fill first half (called from dma half complete) */
	e_SecondHalf, /*!< fill second half (called from dma complete) */
	e_Inv2
} dmaState_t;

typedef enum {
	e_Precursor, /*!< fill full struct */
	e_realData,
	e_Tail_1, /*!<  */
	e_Tail_2, /*!<  */
	e_done,
	e_Inv
}eDmaRawFill;

typedef struct lRawDma_tag
{
	dmaState_t dS;
	eDmaRawFill rS;
	uint32_t iS; /*!< index counter at source */
	uint32_t iD; /*!< index counter at destination */
	const uint32_t ledCount; /*!< count of 'real' leds in the strip */
	const uint32_t rawCount; /*!< size of dma tx buffer (in units of LedRaw[]) */
	const uint32_t rawTxCount; /*!< size of dma tx buffer (in units of uint32_t) */
	LedRaw* lRaw; /*!< pointer to 'real' raw led ctx */
}lRawDma_t;


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

/** \addtogroup Led_Data
 *  @{
 */

/** @brief led chain descriptor, one for each led tube/chain
 *
 */
typedef struct LedChainDesc_tag
{
	LedLogic_t* lLogic; /*!< pointer to led container, i.e. the RGB values of each single LED in a strip */
	lRawDma_t *const lRawNew; /*!< pointer to IO-out raw data of the strip */
	TIM_HandleTypeDef* timer; /*!< pointer to the timer instance responsible for the data output*/
	uint32_t timChannel; /*!< output channel of the timer (as a timer peripheral my have multiple channels) */
	uint32_t rawOn; /*!< pwm setval to produce a set bit*/
	uint32_t rawOff; /*!< pwm setval to produce a reset bit*/
	uint32_t btMult; /*!< brightness truncation multiplier */
	uint32_t btDiv; /*!< brightness truncation divider */
} LedChainDesc_t;

void led_LedLogicInit(LedChainDesc_t* lcd);
void led_setAllLedsToUniColors(LedChainDesc_t* lcd, uint8_t brightness);
void led_setFromToLedsToColor(LedChainDesc_t* lcd, uint8_t r, uint8_t g, uint8_t b, uint32_t s, uint32_t e);
void led_setLedToColor(LedChainDesc_t* lcd, uint32_t i, uint8_t r, uint8_t g, uint8_t b);
void led_getLedColor(LedChainDesc_t* lcd, uint32_t i, LedLogic_t *l);
void led_pasteData(LedChainDesc_t* lcd);
void led_transmitData(LedChainDesc_t* lcd);
void led_txRaw(LedChainDesc_t* lcd);
void led_setBrightnessTruncation(LedChainDesc_t* lcd, uint32_t mult, uint32_t div);
void led_setAllLedsToColor(LedChainDesc_t* lcd, uint8_t r, uint8_t g, uint8_t b);
uint32_t getLedCount(LedChainDesc_t* lcd);

/** @}*/

#endif /* LEDDATA_H_ */
