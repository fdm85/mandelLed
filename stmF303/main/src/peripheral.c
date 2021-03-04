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
#include "adc.h"

void initClock(void)
{
	SystemClock_Config();
}

void initPeripherals(void)
{
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM2_Init();
	MX_ADC1_Init();
	HAL_GPIO_WritePin(MS_RESET_GPIO_Port, MS_RESET_Pin, GPIO_PIN_SET);
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

void msgeq(void)
{
	uint32_t adcState = HAL_ADC_GetState(&hadc1);
	static const uint32_t triggerTimeMs = 50uL;
	static uint32_t lastToggle = 0uL;

	HAL_GPIO_WritePin(MS_RESET_GPIO_Port, MS_RESET_Pin, GPIO_PIN_RESET);
	if(adcState & HAL_ADC_STATE_READY)
	{
		if ((HAL_GetTick() - lastToggle) > triggerTimeMs)
		{
			HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin, GPIO_PIN_SET);
			lastToggle = HAL_GetTick();
			HAL_ADC_Start_IT(&hadc1);
			HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin, GPIO_PIN_RESET);
		}

	}
}

static uint32_t adcVal;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	(void)hadc;
	adcVal = HAL_ADC_GetValue(&hadc1);

}

uint32_t getAdcVal(void)
{
	return adcVal;
}
