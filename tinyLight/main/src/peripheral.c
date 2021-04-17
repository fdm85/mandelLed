/*
 * peripheral.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "peripheral.h"
#include "stdbool.h"
#include "stm32L4xx_hal_gpio.h"

#include "crc.h"
#include "dma.h"
#include "gpio.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

void initClock(void)
{
	SystemClock_Config();
}

void initPeripherals(void)
{
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM4_Init();
	MX_CRC_Init();
	MX_USART2_UART_Init();
}

void greenLedToggle(void)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
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
	return true;
}
