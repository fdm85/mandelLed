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

static LedRaw rawLeds1[RAW_LDCNT];
static LedRaw rawLeds2[RAW_LDCNT];
static LedRaw rawLeds3[RAW_LDCNT];
lRawDma_t matrix_dma = {.ledEnd = MRTX_LDCNT, .ledCountMax = MRTX_LDCNT, .rawCount = RAW_LDCNT, .lRaw = rawLeds1, .rawTxCount = RAW_DMA_CNT};
lRawDma_t mainL_dma = {.ledEnd = MAIN_L_LDCNT, .ledCountMax = MAIN_L_LDCNT, .rawCount = RAW_LDCNT, .lRaw = rawLeds2, .rawTxCount = RAW_DMA_CNT};
lRawDma_t mainR_dma = {.ledEnd = MAIN_R_LDCNT, .ledCountMax = MAIN_R_LDCNT, .rawCount = RAW_LDCNT, .lRaw = rawLeds3, .rawTxCount = RAW_DMA_CNT};

Led_progColor_t mainL_r3[MAIN_L_LDCNT] CCRAM_BSS = {0};
diffRunnerCtx_t mainL_diff CCRAM_DAT = {.lDc = &mainL_r3[0], .size = MAIN_L_LDCNT};

Led_progColor_t mainR_r3[MAIN_R_LDCNT] CCRAM_BSS = {0};
diffRunnerCtx_t mainR_diff CCRAM_DAT = {.lDc = &mainR_r3[0], .size = MAIN_R_LDCNT};

lLogicContainer(mainL, MAIN_L_LDCNT);
lRawContainer(mainL, MAIN_L_LDCNT);
lLogicContainer(mainR, MAIN_R_LDCNT);
lRawContainer(mainR, MAIN_R_LDCNT);

lLogicContainer(matrix, MRTX_LDCNT);
lRawContainer(matrix, MRTX_LDCNT);

LedChainDesc_t lcd_matrix = { \
      .lLogic = &ledsLog_matrix[0], \
      .lRawNew = &matrix_dma, \
      .timer = &htim3, \
      .timChannel = TIM_CHANNEL_1, \
      .diff = NULL,\
  };
LedChainDesc_t lcd_mainR = { \
      .lLogic = &ledsLog_mainR[0], \
      .lRawNew = &mainR_dma, \
      .timer = &htim3, \
      .timChannel = TIM_CHANNEL_2, \
      .diff = &mainR_diff,\
  };
LedChainDesc_t lcd_mainL = { \
      .lLogic = &ledsLog_mainL[0], \
      .lRawNew = &mainL_dma, \
      .timer = &htim3, \
      .timChannel = TIM_CHANNEL_3, \
      .diff = &mainL_diff,\
  };
