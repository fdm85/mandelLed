/******************************************************************************/
/*!
 * \file
 *
 * \brief info: add file brief
 *
 * todo: add file description
 ******************************************************************************/

#include "leds.h"
#include "tim.h"

#define MAIN_LDCNT  256uL

static LedRaw rawLeds[16];
lRawDma_t matrix_dma = {.ledCount = 256uL, .rawCount = 16uL, .lRaw = rawLeds};
lRawDma_t main_dma = {.ledCount = MAIN_LDCNT, .rawCount = (sizeof(rawLeds) / sizeof(rawLeds[0])), .lRaw = rawLeds, .rawTxCount = sizeof(rawLeds)/4};

lLogicContainer(main, MAIN_LDCNT);
//lRawContainer(main, LED_1);
lRawContainer(main, MAIN_LDCNT);
//lChainDesc(main, htim4, TIM_CHANNEL_2, lRawOn, lRawOff);


lLogicContainer(matrix, 256);
lRawContainer(matrix, 256);
//lChainDesc(matrix, htim3, TIM_CHANNEL_1, lRawOn, lRawOff);

	LedChainDesc_t lcd_matrix = { \
				.lLogic = &ledsLog_matrix[0], \
				.lRawNew = &matrix_dma, \
				.timer = &htim4, \
				.timChannel = TIM_CHANNEL_2, \
		};
	LedChainDesc_t lcd_main = { \
				.lLogic = &ledsLog_main[0], \
				.lRawNew = &main_dma, \
				.timer = &htim3, \
				.timChannel = TIM_CHANNEL_1, \
		};
