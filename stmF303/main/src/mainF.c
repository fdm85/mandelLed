/*
 * main.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */


#include "peripheral.h"
#include "ledData.h"
#include "animations.h"
#include "com.h"
#include "cmsis_compiler.h"
#include "stm32f3xx_hal.h"
#include "msgeq7.h"
static void maintainStatusLeds(void)
{
	static const uint32_t blueLedToggleTimeMs = 200uL;
	static uint32_t lastToggle = 0uL;

	if ((HAL_GetTick() - lastToggle) > blueLedToggleTimeMs)
	{
		blueLedToggle();
		lastToggle = HAL_GetTick();

		static uint8_t swCount = 0u;
		if (!getModeSwitch())
		{
			++swCount;
			if (swCount > 5u)
			{
				swCount = 0u;
				orangeLedToggle();
				led_initDataRaw();
//				anim_nextMode();
			}
		}
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
		anim_CyclicCall();
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
		led_pasteData();
		d = HAL_GetTick() - c;
		e = HAL_GetTick();
		led_transmitData();

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
	anim_setMode(anim_cR2);
	led_setBrightnessTruncation(32uL, 255uL);

	led_initDataRaw();
	__enable_irq();
	for (;;)
	{
		maintainStatusLeds();
		msgeq_ticker();
		cyclicReSend();
	}
}
