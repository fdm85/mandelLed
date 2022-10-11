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
#include "stm32f4xx_hal_gpio.h"

#include "crc.h"
#include "dma.h"
#include "gpio.h"
#include "main.h"
#include "main.h"
#include "rng.h"
#include "tim.h"
#include "usart.h"
#include "adc.h"

void initClock(void) {
   SystemClock_Config();
}

void initPeripherals(void) {
   MX_GPIO_Init();
   MX_DMA_Init();
   MX_TIM1_Init();
   MX_TIM3_Init();
   MX_TIM4_Init();
   MX_RNG_Init();
   MX_CRC_Init();
   MX_ADC1_Init();
   MX_ADC2_Init();
   MX_ADC3_Init();
}

void greenLedToggle(void) {
   HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
}

void orangeLedToggle(void) {
   HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}

void blueLedToggle(void) {
   HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
}

bool getModeSwitch(void) {
   return HAL_GPIO_ReadPin(But1_GPIO_Port, But1_Pin);
}

void startEncoder(void) {
   __HAL_TIM_ENABLE(&htim1);
   __HAL_TIM_SET_COUNTER(&htim1, 0u);
}

void stopEncoder(void) {
   __HAL_TIM_DISABLE(&htim1);
}

int16_t getEncoder(void) {
   return (int16_t)__HAL_TIM_GET_COUNTER(&htim1);
}

void brightnessAdc(void) {
   uint32_t adcState = HAL_ADC_GetState(&hadc1);
   static const uint32_t triggerTimeMs = 50uL;
   static uint32_t lastToggle = 0uL;

   if (adcState & HAL_ADC_STATE_READY) {
      if ((HAL_GetTick() - lastToggle) > triggerTimeMs) {
         lastToggle = HAL_GetTick();
         HAL_ADC_Start_IT(&hadc1);
      }

   }
}

static uint32_t adcVal[3] = { 0xFFuL };
void ADC_ConvCpltCallback(void *hadc) {
   static uint8_t i = 0u;
   adcVal[i] = (HAL_ADC_GetValue(hadc) >> 4u);
   ++i;
   if (i == 3u)
      i = 0u;
}

uint32_t getAdcVal(void) {
   uint32_t middle;
   if ((adcVal[0] <= adcVal[1]) && (adcVal[0] <= adcVal[2])) {
      middle = (adcVal[1] <= adcVal[2]) ? adcVal[1] : adcVal[2];
   } else if ((adcVal[1] <= adcVal[0]) && (adcVal[1] <= adcVal[2])) {
      middle = (adcVal[0] <= adcVal[2]) ? adcVal[0] : adcVal[2];
   } else {
      middle = (adcVal[0] <= adcVal[1]) ? adcVal[0] : adcVal[1];
   }
   return (middle);
}
/** @}*/
