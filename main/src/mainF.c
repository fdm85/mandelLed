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
	static const uint32_t triggerTimeMs = 200uL;	static uint32_t lastToggle = 0uL;
	static uint8_t brightness = 0u;
	if( (HAL_GetTick() - lastToggle) > triggerTimeMs)
	{
		led_setAllLedsToUniColors(brightness);
		greenLedToggle();
		led_pasteData();
		led_transmitData();

		brightness += 15u;
		lastToggle = HAL_GetTick();
	}
}


int main(void) {
	initClock();
	initPeripherals();
	led_initDataRaw();
	__enable_irq();

	for (;;) {
		maintainStatusLeds();
		cyclicReSend();
		__NOP();
	}
}
