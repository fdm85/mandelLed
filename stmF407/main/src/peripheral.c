/*
 * peripheral.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "peripheral.h"
#include "stdbool.h"
#include "stm32f4xx_hal_gpio.h"

#include "crc.h"
#include "dma.h"
#include "gpio.h"
#include "main.h"
#include "main.h"
#include "rng.h"
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
	MX_TIM3_Init();
	MX_RNG_Init();
	MX_CRC_Init();
	MX_USART2_UART_Init();
}

void greenLedToggle(void)
{
	HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
}

void orangeLedToggle(void)
{
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}

void blueLedToggle(void)
{
	HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
}


void outputEnableLvlShifter(void)
{
	  // write gpio E0 to low, output-enable of levelshifter --> on
	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
}

bool getModeSwitch(void)
{
	return HAL_GPIO_ReadPin(But1_GPIO_Port, But1_Pin);
}