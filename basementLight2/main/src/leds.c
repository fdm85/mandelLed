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
#define RAW_LDCNT  16uL

static LedRaw rawLeds[RAW_LDCNT];
lRawDma_t matrix_dma = {.ledCount = 1uL, .rawCount = RAW_LDCNT, .lRaw = rawLeds, .rawTxCount = sizeof(rawLeds)/4};
lRawDma_t mainL_dma = {.ledCount = MAIN_LDCNT, .rawCount = RAW_LDCNT, .lRaw = rawLeds, .rawTxCount = sizeof(rawLeds)/4};
lRawDma_t mainR_dma = {.ledCount = MAIN_LDCNT, .rawCount = RAW_LDCNT, .lRaw = rawLeds, .rawTxCount = sizeof(rawLeds)/4};

lLogicContainer(mainL, MAIN_LDCNT);
lRawContainer(mainL, MAIN_LDCNT);
lLogicContainer(mainR, MAIN_LDCNT);
lRawContainer(mainR, MAIN_LDCNT);

lLogicContainer(matrix, 256);
lRawContainer(matrix, 256);

	LedChainDesc_t lcd_matrix = { \
				.lLogic = &ledsLog_matrix[0], \
				.lRawNew = &matrix_dma, \
				.timer = &htim4, \
				.timChannel = TIM_CHANNEL_2, \
		};
	LedChainDesc_t lcd_mainL = { \
				.lLogic = &ledsLog_mainL[0], \
				.lRawNew = &mainL_dma, \
				.timer = &htim3, \
				.timChannel = TIM_CHANNEL_3, \
		};
	LedChainDesc_t lcd_mainR = { \
				.lLogic = &ledsLog_mainR[0], \
				.lRawNew = &mainR_dma, \
				.timer = &htim5, \
				.timChannel = TIM_CHANNEL_4, \
		};
