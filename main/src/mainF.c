/*
 * main.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "cmsis_compiler.h"
#include "stm32f4xx_hal.h"
#include "peripheral.h"
#include "ledData.h"


static void maintainStatusLeds(void)
{
	static const uint32_t blueLedToggleTimeMs = 500uL;	static uint32_t lastToggle = 0uL;

	if( (HAL_GetTick() - lastToggle) > blueLedToggleTimeMs)
	{
		blueLedToggle();
		lastToggle = HAL_GetTick();
	}
}
static void cyclicReSend(void)
{
	static const uint32_t triggerTimeMs = 2000uL;	static uint32_t lastToggle = 0uL;
	static uint8_t innerToggle = 0u;
	if( (HAL_GetTick() - lastToggle) > triggerTimeMs)
	{
		if(++innerToggle %2)
		{
			transmitData();
		}
		else
		{
			transmitData2();
		}
		greenLedToggle();
		lastToggle = HAL_GetTick();
	}
}


int main(void) {
	initClock();
	initPeripherals();
	initDataRaw();
	initDataRaw2();
	__enable_irq();

	for (;;) {
		maintainStatusLeds();
		cyclicReSend();
		__NOP();
	}
}
