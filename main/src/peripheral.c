/*
 * peripheral.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "peripheral.h"
#include "dma.h"
#include "gpio.h"
#include "main.h"
#include "tim.h"

#include "stm32f4xx_hal_gpio.h"

void initClock(void)
{
	SystemClock_Config();
}

void initPeripherals(void)
{
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM3_Init();
}

void greenLedToggle(void)
{
	HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
}

void blueLedToggle(void)
{
	HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
}
