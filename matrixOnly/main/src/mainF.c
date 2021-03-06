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
#include "stm32f3xx_hal.h"
#include "msgeq7.h"
#include "matrix.h"

static void maintainStatusLeds(void)
{
	static const uint32_t blueLedToggleTimeMs = 200uL;
	static uint32_t lastToggle = 0uL;

	if ((HAL_GetTick() - lastToggle) > blueLedToggleTimeMs)
	{
		blueLedToggle();
		lastToggle = HAL_GetTick();
	}
}

typedef enum
{
	e_render, e_waitTxCplt, e_paste
} eSm;
static volatile bool sendLock = false;
static volatile uint32_t a, b, c, d, e, f;
static void cyclicReSend(void)
{

	static eSm state = e_render;

	static const uint32_t triggerTimeMs = 22uL;
	static uint32_t lastToggle = 0uL;

	switch (state)
	{
	case e_render:
		a = HAL_GetTick();
		anim_CyclicCall(&lcd_matrix);
		b = HAL_GetTick() - a;
		state = e_waitTxCplt;
		break;

	case e_waitTxCplt:

		if (!sendLock && ((HAL_GetTick() - lastToggle) > triggerTimeMs))
		{
			state = e_paste;
		}
		break;

	case e_paste:
		greenLedToggle();
		sendLock = true;
		c = HAL_GetTick();
		led_pasteData(&lcd_matrix);
		d = HAL_GetTick() - c;
		e = HAL_GetTick();
		led_transmitData(&lcd_matrix);

		lastToggle = HAL_GetTick();

		state = e_render;
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

	sendLock = false;
	e = HAL_GetTick() - e;

	f = HAL_GetTick();
//  __BKPT(0);
	/* NOTE : This function should not be modified, when the callback is needed,
	 the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
	 */
}

int main(void)
{
	initClock();
	initPeripherals();
	anim_setMode(&lcd_matrix, anim_SpecGraph);
	led_setBrightnessTruncation(&lcd_matrix, 32uL, 255uL);
//	led_setBrightnessTruncation(0x1uL, 0x1uL);
	mtrx_Init();
	led_initDataRaw(&lcd_matrix);
	__enable_irq();
	for (;;)
	{
		maintainStatusLeds();
		msgeq_ticker();
		cyclicReSend();
	}
}
