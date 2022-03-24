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
 */
#include "peripheral.h"
#include "leds.h"
#include "msgeq7.h"
#include "animations.h"
#include "matrix.h"
#include "com.h"
#include "cmsis_compiler.h"
#include "stm32f4xx_hal.h"

/** @brief reads digital IO pin input to switch between
 *  @details Maintains status led on the evaluation board on the fly
 *  @ingroup ComApi
 */
static void maintainStatusLeds(void)
{
	static const uint32_t blueLedToggleTimeMs = 100uL;
	static uint32_t lastToggle = 0uL;

	if ((HAL_GetTick() - lastToggle) > blueLedToggleTimeMs)
	{
		lastToggle = HAL_GetTick();

		static uint8_t swCount = 0u;
		if (!getModeSwitch())
		{
			++swCount;
			if (swCount > 5u)
			{
				swCount = 0u;
				orangeLedToggle();
				led_initDataRaw(&lcd_main);
				anim_nextMode(&lcd_main);
			}
		}
	}
}

mAnim_t anim_main = { .fpRend = anim_CyclicCall, .lcd_ctx = &lcd_main, .triggerTimeMs = 22uL, .puState = init};
mAnim_t anim_matrix = { .fpRend = mtrx_anim, .lcd_ctx = &lcd_matrix, .triggerTimeMs = 10uL, .puState = done};

/** @brief Statemachine to trigger rendering and data output
 *  @param ctx context to work on, brings all needed dependencies
 */
static void cyclicReSend(mAnim_t *ctx)
{

	switch (ctx->state)
	{
	case e_render:
		ctx->a = HAL_GetTick();
		ctx->fpRend(ctx);
		ctx->b = HAL_GetTick() - ctx->a;
		ctx->state = e_waitTxCplt;
		break;

	case e_waitTxCplt:
		if (!ctx->sendLock && ((HAL_GetTick() - ctx->lastToggle) > ctx->triggerTimeMs))
		{
			ctx->state = e_paste;
		}
		break;

	case e_paste:
		ctx->sendLock = 0x55aa55aauL;
		ctx->c = HAL_GetTick();
		led_pasteData(ctx->lcd_ctx);
		ctx->d = HAL_GetTick() - ctx->c;
		ctx->e = HAL_GetTick();
		led_transmitData(ctx->lcd_ctx);

		ctx->lastToggle = HAL_GetTick();

		ctx->state = e_render;
		break;

	default:
		__BKPT(0);
		break;
	}
}

/** @brief DMA timer data out transfer complete ISR implementation
 *  @param htim context of the timer that requested the interrupt
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(htim);
	static mAnim_t* ctx = NULL;

	if(htim == anim_main.lcd_ctx->timer)
		ctx = &anim_main;
	else if(htim == anim_matrix.lcd_ctx->timer)
		ctx = &anim_matrix;
	else
		return;


	ctx->sendLock = 0uL;
	ctx->e = HAL_GetTick() - ctx->e;

	ctx->f = HAL_GetTick();
}

/** @brief main function
 */
int main(void)
{
	static uint32_t brightnessOld;
	static uint32_t brightness;
	initClock();
	initPeripherals();

//	anim_setMode(&lcd_main, anim_min);
	anim_setMode(&lcd_main, anim_msqDrv);
	led_initDataRaw(&lcd_main);
	led_setBrightnessTruncation(&lcd_main, 1uL, 1uL);

	mtrx_Init();
	led_setBrightnessTruncation(&lcd_matrix, 1uL, 1uL);
	led_initDataRaw(&lcd_matrix);

	__enable_irq();

	for (;;)
	{
		maintainStatusLeds();
		msgeq_ticker();
		brightnessAdc();
		brightness = (uint32_t)(0xFFuL & getAdcVal());
		if(brightnessOld != brightness)
		{
			led_setBrightnessTruncation(&lcd_main, brightness, 0xFFu );
			brightnessOld = brightness;
		}
		cyclicReSend(&anim_main);
		cyclicReSend(&anim_matrix);
	}
}
