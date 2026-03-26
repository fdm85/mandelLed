/**
 * @file      mainF.c
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
 * @brief Main implementation of target
 * @defgroup BasementLight LED strip project used for ambient light, featuring music/sound controlled effects
 * @{
 */
#include "peripheral.h"
#include "leds.h"
#include "uartBridge.h"
#include "msgeq7.h"
#include "animations.h"
#include "matrix.h"
#include "com.h"
#include "cmsis_compiler.h"
#include "stm32f4xx_hal.h"
#include "faultHandling.h"
#include <string.h>
#include "BridgeParser.h"

static uint8_t col = 100u;
static uint8_t idx = 0u;
void cycleColors(mAnim_t *ctx) {
  led_setAllLedsToColor(ctx->lcd_ctx, 0, 0, 0);
  for (uint32_t i = ctx->lcd_ctx->lRawNew->ledStart; i < ctx->lcd_ctx->lRawNew->ledEnd;) {
    led_setLedToColor(ctx->lcd_ctx, i++, ((idx + 0) % 3) ? 0 : col, ((idx + 1) % 3) ? 0 : col, ((idx + 2) % 3) ? 0 : col);
    if (i < ctx->lcd_ctx->lRawNew->ledEnd)
      led_setLedToColor(ctx->lcd_ctx, i++, ((idx + 1) % 3) ? 0 : col, ((idx + 2) % 3) ? 0 : col, ((idx + 0) % 3) ? 0 : col);
    if (i < ctx->lcd_ctx->lRawNew->ledEnd)
      led_setLedToColor(ctx->lcd_ctx, i++, ((idx + 2) % 3) ? 0 : col, ((idx + 0) % 3) ? 0 : col, ((idx + 1) % 3) ? 0 : col);
  }
  ++idx;
}
void cycleColorsSingle(mAnim_t *ctx) {
  led_setAllLedsToColor(ctx->lcd_ctx, 0, 0, 0);
  for (uint32_t i = ctx->lcd_ctx->lRawNew->ledStart; i < ctx->lcd_ctx->lRawNew->ledEnd; ++i)
    led_setLedToColor(ctx->lcd_ctx, i, 255u, 255u, 255);
//  led_setLedToColor(ctx->lcd_ctx, i, ((idx + 0) % 3) ? 0 : col, ((idx + 1) % 3) ? 0 : col, ((idx + 2) % 3) ? 0 : col);

  ++idx;
}
void cycleColorsS(mAnim_t *ctx) {
  led_setAllLedsToColor(ctx->lcd_ctx, 0, 0, 0);

  led_setLedToColor(ctx->lcd_ctx, 0, 0, 0, 0);
  led_setLedToColor(ctx->lcd_ctx, 1, 5, 0, 0);
  led_setLedToColor(ctx->lcd_ctx, 2, 0, 5, 0);
  led_setLedToColor(ctx->lcd_ctx, 3, 0, 0, 5);
  led_setLedToColor(ctx->lcd_ctx, 4, 0, 0, 0);
  led_setLedToColor(ctx->lcd_ctx, 5, 5, 5, 5);
  led_setLedToColor(ctx->lcd_ctx, 6, 5, 0, 5);

  led_setLedToColor(ctx->lcd_ctx, 64, 0, 0, 0);
  led_setLedToColor(ctx->lcd_ctx, 65, 5, 0, 0);
  led_setLedToColor(ctx->lcd_ctx, 66, 0, 5, 0);
  led_setLedToColor(ctx->lcd_ctx, 67, 0, 0, 5);
  led_setLedToColor(ctx->lcd_ctx, 68, 0, 0, 0);
  led_setLedToColor(ctx->lcd_ctx, 69, 5, 5, 5);
  led_setLedToColor(ctx->lcd_ctx, 70, 5, 0, 5);

  led_setLedToColor(ctx->lcd_ctx, 255, 0, 0, 0);
  led_setLedToColor(ctx->lcd_ctx, 254, 5, 0, 0);
  led_setLedToColor(ctx->lcd_ctx, 253, 0, 5, 0);
  led_setLedToColor(ctx->lcd_ctx, 252, 0, 0, 5);
  led_setLedToColor(ctx->lcd_ctx, 251, 0, 0, 0);
  led_setLedToColor(ctx->lcd_ctx, 250, 5, 5, 5);
  led_setLedToColor(ctx->lcd_ctx, 249, 5, 0, 5);
}

void allLedsOff(mAnim_t *ctx) {
  led_setAllLedsToColor(ctx->lcd_ctx, 0, 0, 0);
}

static void cycleAnimMainL(mAnim_t *ctx, uint32_t *param, uint8_t isAck) {
  static const void *fpMainL[] = { cycleColorsSingle, anim_frqDrvL, anim_random3, cycleColorsSingle, cycleColors, NULL };
  uint32_t idx;

  if (isAck) {
    idx = *param;
    if (fpMainL[idx] == NULL)
      idx = 0;
    ctx->fpRend = fpMainL[idx];
  }

  for (idx = 0; fpMainL[idx] != NULL; ++idx)
    if (fpMainL[idx] == ctx->fpRend)
      *param = idx;
}

static void cycleAnimMainR(mAnim_t *ctx, uint32_t *param, uint8_t isAck) {
  static const void *fpMainR[] = { cycleColorsSingle, anim_frqDrvR, anim_random3, cycleColorsSingle, cycleColors, NULL };
  uint32_t idx;

  if (isAck) {
    idx = *param;
    if (fpMainR[idx] == NULL)
      idx = 0;
    ctx->fpRend = fpMainR[idx];
  }

  for (idx = 0; fpMainR[idx] != NULL; ++idx)
    if (fpMainR[idx] == ctx->fpRend)
      *param = idx;
}

static void enDisAble(mAnim_t *ctx, uint32_t *param, uint8_t isAck) {
  if (isAck) {
    while (ctx->state < e_Done)
      __NOP();
    ctx->isEnabled ^= 1u;
    ctx->state = (ctx->isEnabled) ? e_reEnable : e_disable;
  }

  *param = (ctx->isEnabled) ? 1uL : 0uL;
}

static void triggerTime(mAnim_t *ctx, uint32_t *param, uint8_t isAck) {
  if (isAck)
    ctx->triggerTime = (param[0] > 100uL) ? param[0] : ctx->triggerTime;

  *param = ctx->triggerTime;
}

static void setBrightnessTruncation(mAnim_t *ctx, uint32_t *param, uint8_t isAck) {

  if (isAck) {
    uint32_t mul = param[0];
    uint32_t div = param[1];
    led_setBrightnessTruncation(ctx->lcd_ctx, mul, div);
  }

  param[0] = ctx->lcd_ctx->btMult;
  param[1] = ctx->lcd_ctx->btDiv;
}

static void setStartAndEnd(mAnim_t *ctx, uint32_t *param, uint8_t isAck) {

  if (isAck)
    led_SetStartAndEnd(ctx->lcd_ctx, param[0], param[1]);

  param[0] = ctx->lcd_ctx->lRawNew->ledStart;
  param[1] = ctx->lcd_ctx->lRawNew->ledEnd;

}

/// .triggerTimeMs = 20000uL == 2 seconds
//mAnim_t anim_main = { .fpRend = cycleColors, .lcd_ctx = &lcd_main, .triggerTimeMs = 1500uL, .puState = done};
mAnim_t anim_mainL = { .fpRend = cycleColorsSingle, .lcd_ctx = &lcd_mainL, .triggerTime = 15000uL, .puState = done, .isEnabled = 1u };
mAnim_t anim_mainR = { .fpRend = cycleColorsSingle, .lcd_ctx = &lcd_mainR, .triggerTime = 15000uL, .puState = done, .isEnabled = 1u };
//mAnim_t anim_matrix = { .fpRend = cycleColorsNone, .lcd_ctx = &lcd_matrix, .triggerTimeMs = 550uL, .puState = done};
mAnim_t anim_matrix = { .fpRend = mtrx_anim, .lcd_ctx = &lcd_matrix, .triggerTime = 5500uL, .puState = done, .isEnabled = 1u };
uint32_t brightnessMainL[2];
uint32_t LedStartEndMainL[2];
uint32_t AnimIdxMainL;
uint32_t enabledMainL;
uint32_t trgTimeMainL;
uint32_t brightnessMainR[2];
uint32_t LedStartEndMainR[2];
uint32_t AnimIdxMainR;
uint32_t enabledMainR;
uint32_t trgTimeMainR;
uint32_t brightnessMatrix[2];
uint32_t enabledMatrix;
uint32_t trgTimeMatrix;

static const uBrdg_Leaf mainL[6] = { { .gtFp = NULL, .des = "anim_mainL", .par = (uint32_t*) &anim_mainL, .pCt = 5 }, { .gtFp = enDisAble, .des = "OnOff",
    .par = &enabledMainL, .pCt = 1 }, { .gtFp = triggerTime, .des = "trgIntervall(100us)", .par = &trgTimeMainL, .pCt = 1 }, { .gtFp = setBrightnessTruncation,
    .des = "Brightness (Mult, Div)", .par = brightnessMainL, .pCt = 2 }, { .gtFp = setStartAndEnd, .des = "activeLeds (start / end)", .par = LedStartEndMainL,
    .pCt = 2 }, { .gtFp = cycleAnimMainL, .des = "fRender (+1;-1)", .par = &AnimIdxMainL, .pCt = 1 }, };
static const uBrdg_Leaf mainR[6] = { { .gtFp = NULL, .des = "anim_mainR", .par = (uint32_t*) &anim_mainR, .pCt = 5 }, { .gtFp = enDisAble, .des = "OnOff",
    .par = &enabledMainR, .pCt = 1 }, { .gtFp = triggerTime, .des = "trgIntervall(100us)", .par = &trgTimeMainR, .pCt = 1 }, { .gtFp = setBrightnessTruncation,
    .des = "Brightness (Mult, Div)", .par = brightnessMainR, .pCt = 2 }, { .gtFp = setStartAndEnd, .des = "activeLeds (start / end)", .par = LedStartEndMainR,
    .pCt = 2 }, { .gtFp = cycleAnimMainR, .des = "fRender (+1;-1)", .par = &AnimIdxMainR, .pCt = 1 }, };
static const uBrdg_Leaf mtrx[6] = { { .gtFp = NULL, .des = "anim_matrix", .par = (uint32_t*) &anim_matrix, .pCt = 3 }, { .gtFp = enDisAble, .des = "OnOff",
    .par = &enabledMatrix, .pCt = 1 }, { .gtFp = triggerTime, .des = "trgIntervall(100us)", .par = &trgTimeMatrix, .pCt = 1 }, {
    .gtFp = setBrightnessTruncation, .des = "Brightness (Mult, Div)", .par = brightnessMatrix, .pCt = 2 },
    { .gtFp = NULL, .des = "N/A", .par = NULL, .pCt = 0 }, { .gtFp = NULL, .des = "N/A", .par = NULL, .pCt = 0 }, };

const uBrdg_Leaf *const leafs[4] = { mainL, mainR, mtrx, NULL };

static void acQueryAll(void) {
  for (uint32_t i = 0; leafs[i] != NULL; ++i) {
    com_TxBuff(leafs[i][0].des, strlen(leafs[i][0].des));
    com_TxBuff("\r", 1);
    for (uint32_t j = 1; j <= leafs[i][0].pCt; ++j) {
      com_TxBuff("   ", 3);
      com_TxBuff(leafs[i][j].des, strlen(leafs[i][j].des));
      com_TxBuff("\r", 1);
    }
  }
}

static void acLeaf(pb_Ctx *const rCtx, pb_Ctx *const tCtx, uint8_t isAck) {
  uint8_t i, ii, j, k, idx = 0u;

  bPar_GetIdcs(&ii, &j, &k);
  i = ii ? ii - 1 : ii;

  if (isAck)
    while (idx < leafs[i][j].pCt) {
      leafs[i][j].par[idx] = pb_Convert(rCtx, NULL);
      ++idx;
    }

  if (*(leafs[i])[j].gtFp)
    leafs[i][j].gtFp((mAnim_t*) leafs[i][0].par, leafs[i][j].par, isAck);

  pb_txPutIndices(tCtx, ii, j, k);
  idx = 0u;
  while (idx < leafs[i][j].pCt) {
    pb_txPutVal(tCtx, leafs[i][j].par[idx]);
    ++idx;
  }
  pb_txPutStr(tCtx, "\r");
  com_Tx();
  bp_ResetRx(rCtx);
}

extern void led_startTransmitData(LedChainDesc_t *lcd);
static void cyclicReSend(mAnim_t *ctx) {

  uint32_t diff;
  static volatile uint32_t lastDiff;
  switch (ctx->state) {
  case e_render:
    if (ctx->isEnabled) {
      ctx->fpRend(ctx);
      ctx->state = e_StartDma;
    } else
      ctx->state = e_disable;

    break;

  case e_StartDma:
    if (TIM_CHANNEL_STATE_GET(ctx->lcd_ctx->timer, ctx->lcd_ctx->timChannel) == HAL_TIM_CHANNEL_STATE_READY) {
      ctx->lcd_ctx->lRawNew->dS = e_fadeIn;
      ctx->lcd_ctx->lRawNew->rS = e_Precursor;
      diff = (HAL_GetTick() - ctx->lastToggle);
      if ((diff >= ctx->triggerTime)) {
        lastDiff = diff;
        ctx->lastToggle = HAL_GetTick();
        led_txRaw(ctx->lcd_ctx);
        ctx->state = e_waitDmaDone;
        (void)lastDiff;
      }
    }
    break;

  case e_waitDmaDone:
    if (ctx->lcd_ctx->lRawNew->rS == e_done)
      ctx->state = e_Done;
    break;

  case e_reEnable:
    ctx->lcd_ctx->lRawNew->rS = e_done;
    led_stopTransmitData(ctx->lcd_ctx);
    ctx->state = e_render;
    break;
  case e_disabled:
    break;
  case e_disable:
    ctx->lcd_ctx->lRawNew->rS = e_done;
    led_stopTransmitData(ctx->lcd_ctx);
    allLedsOff(ctx);
    ctx->state = e_disabled;
    break;
  case e_Done:
    ctx->state = ctx->isEnabled ? e_render : e_disabled;
    break;

  default:
    __BKPT(0);
    break;
  }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
  LedChainDesc_t *lcd = &lcd_matrix;

  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    lcd = &lcd_mainR;
  if (htim == &htim4)
    lcd = &lcd_mainL;

  lcd->lRawNew->dS = e_FirstHalf;
  led_txRaw(lcd);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  LedChainDesc_t *lcd = &lcd_matrix;

  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    lcd = &lcd_mainR;
  if (htim == &htim4)
    lcd = &lcd_mainL;

  lcd->lRawNew->dS = e_SecondHalf;
  led_txRaw(lcd);
}

static void maintainStatusLeds(void) {
  static const uint32_t blueLedToggleTimeMs = 100uL;
  static uint32_t lastToggle = 0uL;
  static uint8_t mrtxPuState = 0u;
  if ((HAL_GetTick() - lastToggle) > blueLedToggleTimeMs) {
    lastToggle = HAL_GetTick();

    static uint8_t swCount = 0u;
    if (getModeSwitch()) {
      ++swCount;
      if (swCount > 7u) {
        swCount = 0u;
        orangeLedToggle();

        if (mrtxPuState < 5)
          ++mrtxPuState;
        if (mrtxPuState == 1)
          enDisAble(&anim_matrix, &enabledMatrix, 1u);
        else if (mrtxPuState == 2) {
          anim_mainL.triggerTime = 550uL;
          anim_mainL.fpRend = anim_frqDrvL;
          anim_mainR.triggerTime = 550uL;
          anim_mainR.fpRend = anim_frqDrvR;
        } else if (mrtxPuState == 3)
          enDisAble(&anim_matrix, &enabledMatrix, 1u);
        else if (mrtxPuState == 4) {
          anim_mainL.triggerTime = 1000uL;
          anim_mainL.fpRend = anim_random3;
          anim_mainR.triggerTime = 1000uL;
          anim_mainR.fpRend = anim_random3;
        } else {
          if (anim_mainL.lcd_ctx->lRawNew->ledEnd > 20)
            anim_mainL.lcd_ctx->lRawNew->ledEnd -= 20;
          else
            anim_mainL.lcd_ctx->lRawNew->ledEnd = anim_mainL.lcd_ctx->lRawNew->ledCountMax;

          if (anim_mainR.lcd_ctx->lRawNew->ledEnd > 20)
            anim_mainR.lcd_ctx->lRawNew->ledEnd -= 20;
          else {
            anim_mainL.lcd_ctx->lRawNew->ledEnd = anim_mainL.lcd_ctx->lRawNew->ledCountMax;
            anim_mainR.lcd_ctx->lRawNew->ledEnd = anim_mainR.lcd_ctx->lRawNew->ledCountMax;
            mrtxPuState = 0u;
          }
        }
      }
    }
  }
}
/** @brief main function
 */
int main(void) {
  pb_ParserState parStt;
  initClock();
  initPeripherals();
  com_SetDump();

  led_setBrightnessTruncation(anim_mainL.lcd_ctx, 1uL, 1uL);
  led_setBrightnessTruncation(anim_mainR.lcd_ctx, 1uL, 1uL);
  led_setBrightnessTruncation(anim_matrix.lcd_ctx, 1uL, 1uL);

  mtrx_Init();
	led_LedLogicInit(anim_mainL.lcd_ctx);
	led_LedLogicInit(anim_mainR.lcd_ctx);
  led_LedLogicInit(anim_matrix.lcd_ctx);

  __enable_irq();

  com_Init();

  for (;;) {
    maintainStatusLeds();
    msgeq_ticker();
    parStt = bp_Parse(&rxCtx);
    if (parStt > pb_eTimeOut)
      acLeaf(&rxCtx, &txCtx, parStt == pb_eAck ? 1u : 0u);

		cyclicReSend(&anim_mainL);
		cyclicReSend(&anim_mainR);
    cyclicReSend(&anim_matrix);
  }
}

void _close(void) {
}

void _lseek(void) {

}

void _read(void) {
}

void _write(void) {
}

/** @}*/
