/*
 * main.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "peripheral.h"
#include "leds.h"
#include "animations.h"
#include "com.h"
#include "cmsis_compiler.h"

#include "stm32l4xx_hal.h"

static void maintainModeSwitch(void)
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
				led_initDataRaw(&lcd_main);
				anim_nextMode(&lcd_main);
			}
		}
	}
}

mAnim_t anim_main = { .fpRend = anim_CyclicCall, .lcd_ctx = &lcd_main, .triggerTimeMs = 2000uL, .puState = done};

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

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(htim);

	anim_main.sendLock = false;
	anim_main.e = HAL_GetTick() - anim_main.e;

	anim_main.f = HAL_GetTick();
//  __BKPT(0);
	/* NOTE : This function should not be modified, when the callback is needed,
	 the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
	 */
}

int main(void)
{
	initClock();
	initPeripherals();

	anim_setMode(&lcd_main, anim_white);
	led_initDataRaw(&lcd_main);
	led_setBrightnessTruncation(&lcd_main, 1uL, 1uL);

	__enable_irq();
	for (;;)
	{
		maintainModeSwitch();
		cyclicReSend(&anim_main);
	}
}
