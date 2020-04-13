/*
 * main.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "cmsis_compiler.h"
#include "stm32f4xx_hal.h"
#include "peripheral.h"


static void maintainStatusLeds(void)
{
	static const uint32_t blueLedToggleTimeMs = 500uL;	static uint32_t lastToggle = 0uL;

	if( (HAL_GetTick() - lastToggle) > blueLedToggleTimeMs)
	{
		blueLedToggle();
		lastToggle = HAL_GetTick();
	}
}


int main(void) {
	initClock();
	initPeripherals();
	__enable_irq();
	for (;;) {
		maintainStatusLeds();
		__NOP();
	}
}
