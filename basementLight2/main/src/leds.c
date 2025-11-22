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

#define MAIN_LDCNT  750uL
#define MRTX_LDCNT  256uL
#define RAW_LDCNT  16uL
#define RAW_DMA_CNT (3uL*8uL*RAW_LDCNT)

static LedRaw rawLeds1[RAW_LDCNT];
static LedRaw rawLeds2[RAW_LDCNT];
static LedRaw rawLeds3[RAW_LDCNT];
lRawDma_t matrix_dma = {.ledCount = MRTX_LDCNT, .rawCount = RAW_LDCNT, .lRaw = rawLeds1, .rawTxCount = RAW_DMA_CNT};
lRawDma_t mainL_dma = {.ledCount = MAIN_LDCNT, .rawCount = RAW_LDCNT, .lRaw = rawLeds2, .rawTxCount = RAW_DMA_CNT};
lRawDma_t mainR_dma = {.ledCount = MAIN_LDCNT, .rawCount = RAW_LDCNT, .lRaw = rawLeds3, .rawTxCount = RAW_DMA_CNT};

Led_progColor_t mainL_r3[MAIN_LDCNT] __attribute__ ((section (".bssmram")));
diffRunnerCtx_t mainL_diff __attribute__ ((section (".ccmram"))) = {.lDc = &mainL_r3[0], .size = MAIN_LDCNT};

Led_progColor_t mainR_r3[MAIN_LDCNT] __attribute__ ((section (".bssmram")));
diffRunnerCtx_t mainR_diff __attribute__ ((section (".ccmram"))) = {.lDc = &mainR_r3[0], .size = MAIN_LDCNT};

lLogicContainer(mainL, MAIN_LDCNT);
lRawContainer(mainL, MAIN_LDCNT);
lLogicContainer(mainR, MAIN_LDCNT);
lRawContainer(mainR, MAIN_LDCNT);

lLogicContainer(matrix, MRTX_LDCNT);
lRawContainer(matrix, MRTX_LDCNT);

	LedChainDesc_t lcd_matrix = { \
				.lLogic = &ledsLog_matrix[0], \
				.lRawNew = &matrix_dma, \
				.timer = &htim3, \
				.timChannel = TIM_CHANNEL_3, \
				.diff = NULL,\
		};
	LedChainDesc_t lcd_mainR = { \
				.lLogic = &ledsLog_mainR[0], \
				.lRawNew = &mainR_dma, \
				.timer = &htim4, \
				.timChannel = TIM_CHANNEL_2, \
				.diff = &mainR_diff,\
		};
	LedChainDesc_t lcd_mainL = { \
				.lLogic = &ledsLog_mainL[0], \
				.lRawNew = &mainL_dma, \
				.timer = &htim3, \
				.timChannel = TIM_CHANNEL_2, \
				.diff = &mainL_diff,\
		};
