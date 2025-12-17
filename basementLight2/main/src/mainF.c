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


static uint8_t col = 5;
static uint8_t idx = 0;
void cycleColors(mAnim_t* ctx)
{
  for (uint32_t i = ctx->lcd_ctx->lRawNew->ledStart; i < ctx->lcd_ctx->lRawNew->ledEnd;) {
    led_setLedToColor(ctx->lcd_ctx, i++, ((idx + 0) % 3) ? 0 : col, ((idx + 1) % 3) ? 0 : col, ((idx + 2) % 3) ? 0 : col);
    if(i < ctx->lcd_ctx->lRawNew->ledEnd)
      led_setLedToColor(ctx->lcd_ctx, i++, ((idx + 1) % 3) ? 0 : col, ((idx + 2) % 3) ? 0 : col, ((idx + 0) % 3) ? 0 : col);
    if(i < ctx->lcd_ctx->lRawNew->ledEnd)
      led_setLedToColor(ctx->lcd_ctx, i++, ((idx + 2) % 3) ? 0 : col, ((idx + 0) % 3) ? 0 : col, ((idx + 1) % 3) ? 0 : col);
  }
  ++idx;
}
void cycleColorsSingle(mAnim_t* ctx)
{
  for (uint32_t i = ctx->lcd_ctx->lRawNew->ledStart; i < ctx->lcd_ctx->lRawNew->ledEnd; ++i)
    led_setLedToColor(ctx->lcd_ctx, i, ((idx + 0) % 3) ? 0 : col, ((idx + 1) % 3) ? 0 : col, ((idx + 2) % 3) ? 0 : col);

  ++idx;
}
void cycleColorsS(mAnim_t* ctx)
{
  led_setAllLedsToColor(ctx->lcd_ctx, 0,0,0);

  led_setLedToColor(ctx->lcd_ctx, 0, 0,0,0);
  led_setLedToColor(ctx->lcd_ctx, 1, 5,0,0);
  led_setLedToColor(ctx->lcd_ctx, 2, 0,5,0);
  led_setLedToColor(ctx->lcd_ctx, 3, 0,0,5);
  led_setLedToColor(ctx->lcd_ctx, 4, 0,0,0);
  led_setLedToColor(ctx->lcd_ctx, 5, 5,5,5);
  led_setLedToColor(ctx->lcd_ctx, 6, 5,0,5);

  led_setLedToColor(ctx->lcd_ctx, 64, 0,0,0);
  led_setLedToColor(ctx->lcd_ctx, 65, 5,0,0);
  led_setLedToColor(ctx->lcd_ctx, 66, 0,5,0);
  led_setLedToColor(ctx->lcd_ctx, 67, 0,0,5);
  led_setLedToColor(ctx->lcd_ctx, 68, 0,0,0);
  led_setLedToColor(ctx->lcd_ctx, 69, 5,5,5);
  led_setLedToColor(ctx->lcd_ctx, 70, 5,0,5);

  led_setLedToColor(ctx->lcd_ctx, 255, 0,0,0);
  led_setLedToColor(ctx->lcd_ctx, 254, 5,0,0);
  led_setLedToColor(ctx->lcd_ctx, 253, 0,5,0);
  led_setLedToColor(ctx->lcd_ctx, 252, 0,0,5);
  led_setLedToColor(ctx->lcd_ctx, 251, 0,0,0);
  led_setLedToColor(ctx->lcd_ctx, 250, 5,5,5);
  led_setLedToColor(ctx->lcd_ctx, 249, 5,0,5);
}

void allLedsOff(mAnim_t* ctx)
{
  led_setAllLedsToColor(ctx->lcd_ctx, 0,0,0);
}

static void cycleAnimMainL(mAnim_t *ctx, const void * param){
  static const void* fpMainL[] =  {anim_frqDrvL, anim_random3, cycleColorsSingle, cycleColors, NULL};
  uint8_t idx = *(uint8_t *)param;
  if(idx >= (sizeof(fpMainL)/sizeof(fpMainL[0])))
    idx = 0;
  ctx->fpRend = fpMainL[idx];
}

static void cycleAnimMainR(mAnim_t *ctx, const void * param){
  static const void* fpMainR[] =  {anim_frqDrvR, anim_random3, cycleColorsSingle, cycleColors, NULL};
  uint8_t idx = *(uint8_t *)param;
  if(idx >= (sizeof(fpMainR)/sizeof(fpMainR[0])))
    idx = 0;
  ctx->fpRend = fpMainR[idx];
}

static void setBrightnessTruncation(mAnim_t *ctx, const void * param){
  uint32_t mul = ((uint32_t *)param)[0];
  uint32_t div = ((uint32_t *)param)[1];
  led_setBrightnessTruncation(ctx->lcd_ctx, mul, div);
}

static void setStartAndEnd(mAnim_t *ctx, const void * param){
  uint32_t start = ((uint32_t *)param)[0];
  uint32_t end = ((uint32_t *)param)[1];
  led_SetStartAndEnd(ctx->lcd_ctx, start, end);
}

/// .triggerTimeMs = 20000uL == 2 seconds
//mAnim_t anim_main = { .fpRend = cycleColors, .lcd_ctx = &lcd_main, .triggerTimeMs = 1500uL, .puState = done};
mAnim_t anim_mainL = { .fpRend = anim_random3, .lcd_ctx = &lcd_mainL, .triggerTimeMs = 1000uL, .puState = done, .isEnabled = 1u};
mAnim_t anim_mainR = { .fpRend = anim_random3, .lcd_ctx = &lcd_mainR, .triggerTimeMs = 1000uL, .puState = done, .isEnabled = 1u};
//mAnim_t anim_matrix = { .fpRend = cycleColorsNone, .lcd_ctx = &lcd_matrix, .triggerTimeMs = 550uL, .puState = done};
mAnim_t anim_matrix = { .fpRend = anim_random3, .lcd_ctx = &lcd_matrix, .triggerTimeMs = 550uL, .puState = done, .isEnabled = 0u};
uint32_t brightnessMainL[2];
uint32_t LedStartEndMainL[2];
uint8_t AnimIdxMainL;
uint32_t brightnessMainR[2];
uint32_t LedStartEndMainR[2];
uint8_t AnimIdxMainR;

static const uBrdg_Leaf mainL[6] = { {.gtFp = NULL, .des = "anim_mainL", .par = &anim_mainL, .pCt = 5},
                              {.gtFp = NULL, .des = "OnOff", .par = &anim_mainL.isEnabled, .pCt = 1},
                              {.gtFp = NULL, .des = "trgIntervall(100us)", .par = &anim_mainL.triggerTimeMs, .pCt = 1},
                              {.gtFp = setBrightnessTruncation, .des = "Brightness (Mult, Div)", .par = brightnessMainL, .pCt = 2},
                              {.gtFp = setStartAndEnd, .des = "activeLeds (start / end)", .par = LedStartEndMainL, .pCt = 2},
                              {.gtFp = cycleAnimMainL, .des = "fRender (+1;-1)", .par = &AnimIdxMainL, .pCt = 1},
};
static const uBrdg_Leaf mainR[6] = { {.gtFp = NULL, .des = "anim_mainR", .par = &anim_mainR, .pCt = 5},
                              {.gtFp = NULL, .des = "OnOff", .par = &anim_mainR.isEnabled, .pCt = 1},
                              {.gtFp = NULL, .des = "trgIntervall(100us)", .par = &anim_mainR.triggerTimeMs, .pCt = 1},
                              {.gtFp = setBrightnessTruncation, .des = "Brightness (Mult, Div)", .par = brightnessMainR, .pCt = 2},
                              {.gtFp = setStartAndEnd, .des = "activeLeds (start / end)", .par = LedStartEndMainR, .pCt = 2},
                              {.gtFp = cycleAnimMainR, .des = "fRender (+1;-1)", .par = &AnimIdxMainR, .pCt = 1},
};

const uBrdg_Leaf *const leafs[3] = {
    mainL, mainR, NULL
};

extern void led_startTransmitData(LedChainDesc_t* lcd);
static void cyclicReSend(mAnim_t *ctx) {

    switch (ctx->state) {
    case e_render:
      if(ctx->isEnabled)
        ctx->fpRend(ctx);
      else
        allLedsOff(ctx);
      ctx->state = e_StartDma;
      break;

    case e_StartDma:
      if(((HAL_GetTick() - ctx->lastToggle) < ctx->triggerTimeMs))
        return;
      ctx->lastToggle = HAL_GetTick();
      ctx->lcd_ctx->lRawNew->dS = e_fadeIn;
      ctx->lcd_ctx->lRawNew->rS = e_Precursor;
      led_txRaw(ctx->lcd_ctx);
      ctx->state = e_waitDmaDone;
      break;

    case e_waitDmaDone:
      if (ctx->lcd_ctx->lRawNew->rS == e_done)
        ctx->state = e_render;
      break;

    case e_waitTxCplt:
    case e_paste:
      break;

    default:
      __BKPT(0);
      break;
    }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  LedChainDesc_t* lcd = htim->ctx;

	lcd->lRawNew->dS = e_FirstHalf;
//	HAL_GPIO_TogglePin(dbg1_GPIO_Port, dbg1_Pin);
	led_txRaw(lcd);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  LedChainDesc_t* lcd = htim->ctx;

	lcd->lRawNew->dS = e_SecondHalf;
//	HAL_GPIO_TogglePin(dbg1_GPIO_Port, dbg1_Pin);
	led_txRaw(lcd);
//	else
//	{
//		anim_main.sendLock = false;
//		anim_main.e = HAL_GetTick() - anim_main.e;
//		anim_main.f = HAL_GetTick();
//	}
//  __BKPT(0);
	/* NOTE : This function should not be modified, when the callback is needed,
	 the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
	 */
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
      if (swCount > 5u) {
        swCount = 0u;
        orangeLedToggle();

        if (mrtxPuState < 5)
          ++mrtxPuState;
        if (mrtxPuState == 1)
          anim_matrix.fpRend = mtrx_anim;
        else if (mrtxPuState == 2)
        {
          anim_mainL.triggerTimeMs = 550uL;
          anim_mainL.fpRend = anim_frqDrvL;
          anim_mainR.triggerTimeMs = 550uL;
          anim_mainR.fpRend = anim_frqDrvR;
        }
        else if (mrtxPuState == 3)
          anim_matrix.fpRend = allLedsOff;
        else if (mrtxPuState == 4)
        {
          anim_mainL.triggerTimeMs = 1000uL;
          anim_mainL.fpRend = anim_random3;
          anim_mainR.triggerTimeMs = 1000uL;
          anim_mainR.fpRend = anim_random3;
        }
        else {
          if (anim_mainL.lcd_ctx->lRawNew->ledEnd > 20)
            anim_mainL.lcd_ctx->lRawNew->ledEnd -= 20;
          else
            anim_mainL.lcd_ctx->lRawNew->ledEnd = anim_mainL.lcd_ctx->lRawNew->ledCountMax;

          if (anim_mainR.lcd_ctx->lRawNew->ledEnd > 20)
            anim_mainR.lcd_ctx->lRawNew->ledEnd -= 20;
          else
          {
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
int main(void)
{
	initClock();
	initPeripherals();
	com_SetDump();

	led_setBrightnessTruncation(anim_mainL.lcd_ctx, 1uL, 1uL);
	led_setBrightnessTruncation(anim_mainR.lcd_ctx, 1uL, 1uL);
	led_setBrightnessTruncation(anim_matrix.lcd_ctx, 1uL, 1uL);

	mtrx_Init();
	led_LedLogicInit(anim_matrix.lcd_ctx);
	led_LedLogicInit(anim_mainL.lcd_ctx);
	led_LedLogicInit(anim_mainR.lcd_ctx);

	__enable_irq();

	com_Init();

	for (;;)
	{
	  maintainStatusLeds();
		msgeq_ticker();
		bp_Parse(&rxCtx);
		cyclicReSend(&anim_matrix);
		cyclicReSend(&anim_mainL);
		cyclicReSend(&anim_mainR);
	}
}

void _close(void)
{
}

void _lseek(void)
{

}

void _read(void)
{
}

void _write(void)
{
}

/** @}*/
