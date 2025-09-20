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
#include "msgeq7.h"
#include "animations.h"
#include "matrix.h"
#include "com.h"
#include "cmsis_compiler.h"
#include "stm32f4xx_hal.h"

//mAnim_t anim_main = { .fpRend = cycleColors, .lcd_ctx = &lcd_main, .triggerTimeMs = 1500uL, .puState = done};
mAnim_t anim_main = { .fpRend = mtrx_anim, .lcd_ctx = &lcd_main, .triggerTimeMs = 10uL, .puState = done};
mAnim_t anim_matrix = { .fpRend = mtrx_anim, .lcd_ctx = &lcd_matrix, .triggerTimeMs = 10uL, .puState = done};

extern void led_startTransmitData(LedChainDesc_t* lcd);
static void cyclicReSend(mAnim_t *ctx) {

  switch (ctx->state) {
  case e_render:
    ctx->a = HAL_GetTick();
    ctx->fpRend(ctx);
    ctx->b = HAL_GetTick() - ctx->a;
    ctx->state = e_StartDma;
    break;

  case e_StartDma:
//		ctx->c = HAL_GetTick();
//		ctx->d = HAL_GetTick() - ctx->c;
//		ctx->e = HAL_GetTick();
    ctx->lastToggle = HAL_GetTick();
    ctx->lcd_ctx->lRawNew->dS = e_fadeIn;
    ctx->lcd_ctx->lRawNew->rS = e_Precursor;
    initMea();
    led_txRaw(ctx->lcd_ctx);
    ctx->state = e_waitDmaDone;
    break;

  case e_waitDmaDone:
    if ((ctx->lcd_ctx->lRawNew->rS == e_done)
        && ((HAL_GetTick() - ctx->lastToggle) > ctx->triggerTimeMs))
      ctx->state = e_render;
    break;

  default:
    __BKPT(0);
    break;
  }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	LedChainDesc_t* lcd = (htim == &htim3) ? &lcd_matrix : &lcd_main;
	lcd->lRawNew->dS = e_FirstHalf;
//	HAL_GPIO_TogglePin(dbg1_GPIO_Port, dbg1_Pin);
	led_txRaw(lcd);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	LedChainDesc_t* lcd = (htim == &htim3) ? &lcd_matrix : &lcd_main;
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

/** @brief main function
 */
int main(void)
{
	initClock();
	initPeripherals();

	anim_main.AnimMode = anim_rnd3;
	anim_matrix.AnimMode = anim_msqDrv;
//	anim_setMode(&lcd_main, anim_rnd3);
	led_setBrightnessTruncation(&lcd_main, 1uL, 1uL);

	mtrx_Init();
//	led_setBrightnessTruncation(&lcd_matrix, 1uL, 1uL);
	led_LedLogicInit(&lcd_main);
//	mtrx_Init();
//	led_LedLogicInit(&lcd_matrix);

	__enable_irq();
	for (;;)
	{
//		maintainModeSwitch();
		msgeq_ticker();
		cyclicReSend(&anim_main);
//		cyclicReSend(&anim_matrix);
	}
}
/** @}*/
