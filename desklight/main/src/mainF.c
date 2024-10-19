/*
 * main.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "peripheral.h"
#include "leds.h"
#include "animations.h"
#include "matrix.h"
#include "com.h"
#include "cmsis_compiler.h"
#include "crc.h"
#include "stm32f1xx_hal_conf.h"

//static void maintainModeSwitch(void)
//{
//	static const uint32_t blueLedToggleTimeMs = 100uL;
//	static uint32_t lastToggle = 0uL;
//
//	if ((HAL_GetTick() - lastToggle) > blueLedToggleTimeMs)
//	{
//		lastToggle = HAL_GetTick();
//
//		static uint8_t swCount = 0u;
//		if (!getModeSwitch())
//		{
//			++swCount;
//			if (swCount > 5u)
//			{
//				swCount = 0u;
//				led_initDataRaw(&lcd_main);
//				anim_nextMode(&lcd_main);
//			}
//		}
//	}
//}

mAnim_t anim_main = { .fpRend = anim_CyclicCall, .lcd_ctx = &lcd_main, .triggerTimeMs = 100uL, .puState = done};
mAnim_t anim_matrix = { .fpRend = mtrx_anim, .lcd_ctx = &lcd_matrix, .triggerTimeMs = 10uL, .puState = done};

static void cyclicReSend(mAnim_t *ctx)
{

	switch (ctx->state)
	{
	case e_render:
		ctx->a = HAL_GetTick();
		ctx->fpRend(ctx);
		ctx->b = HAL_GetTick() - ctx->a;
//		ctx->state = e_waitTxCplt;
		ctx->state = e_StartDma;
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
//		led_pasteData(ctx->lcd_ctx);
		ctx->d = HAL_GetTick() - ctx->c;
		ctx->e = HAL_GetTick();
//		led_transmitData(ctx->lcd_ctx);

		ctx->lastToggle = HAL_GetTick();

		ctx->state = e_render;
		break;

	case e_StartDma:
		led_txRaw(ctx->lcd_ctx, e_Init);
		ctx->state = e_waitDmaDone;
		break;

	case e_waitDmaDone:
		break;

	default:
		__BKPT(0);
		break;
	}
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	LedChainDesc_t* lcd = (htim == &htim3) ? &lcd_matrix : &lcd_main;
	led_txRaw(lcd, e_FirstHalf);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	LedChainDesc_t* lcd = (htim == &htim3) ? &lcd_matrix : &lcd_main;
	led_txRaw(lcd, e_SecondHalf);

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

int main(void)
{
	initClock();
	initPeripherals();
	MX_CRC_Init();
	anim_main.AnimMode = anim_rnd3;
	anim_matrix.AnimMode = anim_msqDrv;
//	anim_setMode(&lcd_main, anim_rnd3);
	led_setBrightnessTruncation(&lcd_main, 1uL, 1uL);
//	led_setBrightnessTruncation(&lcd_matrix, 1uL, 1uL);
	led_LedLogicInit(&lcd_main);
	led_setLedToColor(&lcd_main, 0, 255, 0, 0);
	led_setLedToColor(&lcd_main, 1, 0, 255, 0);
	led_setLedToColor(&lcd_main, 2, 0, 0, 255);
	led_setLedToColor(&lcd_main, 3, 255, 0, 0);
	led_setLedToColor(&lcd_main, 4, 0, 255, 0);
	led_setLedToColor(&lcd_main, 5, 0, 0, 255);
//	led_LedLogicInit(&lcd_matrix);
//	mtrx_Init();
	__enable_irq();
	for (;;)
	{
//		maintainModeSwitch();
//		msgeq_ticker();
		cyclicReSend(&anim_main);
//		cyclicReSend(&anim_matrix);
	}
}

uint32_t hrng;

HAL_CRC_StateTypeDef HAL_RNG_GenerateRandomNumber(void * vd, uint32_t *random32bit)
{
	(void)vd;
	uint32_t temp = HAL_GetTick();

	*random32bit = HAL_CRC_Accumulate(&hcrc, &temp, 1);

	return  hcrc.State;
}
