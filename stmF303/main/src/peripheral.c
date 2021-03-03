/*
 * peripheral.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "peripheral.h"
#include "stdbool.h"
#include "stm32f3xx_hal_gpio.h"

#include "dma.h"
#include "gpio.h"
#include "main.h"
#include "tim.h"

void initClock(void)
{
	SystemClock_Config();
}

void initPeripherals(void)
{
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM2_Init();
}

void greenLedToggle(void)
{
}

void orangeLedToggle(void)
{
}

void blueLedToggle(void)
{
}


void outputEnableLvlShifter(void)
{
}

bool getModeSwitch(void)
{
	return false;
}
