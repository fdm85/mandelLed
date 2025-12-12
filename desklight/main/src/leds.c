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
lRawDma_t main_dma = {.ledCount = MAIN_LDCNT, .ledCountMax = MAIN_LDCNT, .rawCount = RAW_LDCNT, .lRaw = rawLeds1, .rawTxCount = RAW_DMA_CNT};
Led_progColor_t main_r3[MAIN_LDCNT] = {0};
diffRunnerCtx_t main_diff = {.lDc = &main_r3[0], .size = MAIN_LDCNT};
lLogicContainer(main, MAIN_LDCNT);
lRawContainer(main, MAIN_LDCNT);

LedChainDesc_t lcd_main = { \
      .lLogic = &ledsLog_main[0], \
      .lRawNew = &main_dma, \
      .timer = &htim3, \
      .timChannel = TIM_CHANNEL_1, \
      .diff = &main_diff,\
  };
