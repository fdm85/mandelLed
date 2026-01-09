/**
 * @file      peripheral.c
 * @authors   Clemens Grünberger
 * @copyright 2022  Clemens Grünberger
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @brief Implementation of peripheral interface
 * @ingroup PeripheralInterface
 * @ingroup BasementLight
 * @{
 */

#include "peripheral.h"
#include "stdbool.h"
//#include "stm32f4xx_hal_conf.h"

#include "dma.h"
#include "gpio.h"
#include "main.h"
#include "rng.h"
#include "tim.h"
#include "usart.h"
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
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_RNG_Init();
	MX_ADC2_Init();
	MX_ADC3_Init();
	MX_USART2_UART_Init();
}

uint32_t HAL_GetTick(void)
{
  return __HAL_TIM_GET_COUNTER(&htim2);
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

bool getModeSwitch(void)
{
	return HAL_GPIO_ReadPin(But1_GPIO_Port, But1_Pin);
}

static uint32_t adcVal[3] =
{ 0xFFuL };
void ADC_ConvCpltCallback(void *hadc)
{
	static uint8_t i = 0u;
	adcVal[i] = (HAL_ADC_GetValue(hadc) >> 4u);
	++i;
	if (i == 3u)
		i = 0u;
}

uint32_t getAdcVal(void)
{
	uint32_t middle;
	if ((adcVal[0] <= adcVal[1]) && (adcVal[0] <= adcVal[2]))
	{
		middle = (adcVal[1] <= adcVal[2]) ? adcVal[1] : adcVal[2];
	}
	else if ((adcVal[1] <= adcVal[0]) && (adcVal[1] <= adcVal[2]))
	{
		middle = (adcVal[0] <= adcVal[2]) ? adcVal[0] : adcVal[2];
	}
	else
	{
		middle = (adcVal[0] <= adcVal[1]) ? adcVal[0] : adcVal[1];
	}
	return (middle);
}
/** @}*/
