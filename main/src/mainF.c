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
	static const uint32_t blueLedToggleTimeMs = 200uL;	static uint32_t lastToggle = 0uL;

	if( (HAL_GetTick() - lastToggle) > blueLedToggleTimeMs)
	{
		blueLedToggle();
		lastToggle = HAL_GetTick();
	}
}
static void cyclicReSend(void)
{
	static const uint32_t triggerTimeMs = 50uL;	static uint32_t lastToggle = 0uL;
	static uint8_t brightness = 15u;
	static uint8_t index = 0u;
	if( (HAL_GetTick() - lastToggle) > triggerTimeMs)
	{
		led_setAllLedsToUniColors(brightness);
		led_setLedToColor(index, 0u,0u, brightness);
		led_setLedToColor((index + 1u), 0u,brightness, 0u);
		led_setLedToColor((index + 2u), brightness,0u, 0u);
		led_setLedToColor(index + 20u, 0u,0u, brightness);
		led_setLedToColor((index + 21u), 0u,brightness, 0u);
		led_setLedToColor((index + 22u), brightness,0u, 0u);
		led_setLedToColor(index + 40u, 0u,0u, brightness);
		led_setLedToColor((index + 41u), 0u,brightness, 0u);
		led_setLedToColor((index + 42u), brightness,0u, 0u);
		greenLedToggle();
		led_pasteData();
		led_transmitData();

		++index;
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
