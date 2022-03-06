/*
 * main.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "peripheral.h"
#include "leds.h"
#include "msgeq7.h"
#include "animations.h"
#include "matrix.h"
#include "com.h"
#include "cmsis_compiler.h"
#include "stm32f4xx_hal.h"


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
