/*
 * ledData.c
 *
 *  Created on: 13.04.2020
 *      Author: cgrue
 */

#include "ledData.h"
#include "stdint.h"
#include "tim.h"
#define ledRawSize ((3 * 8))
#define retLength 40u
#define length (ledRawSize + retLength)

static uint16_t data[length];

void initDataRaw(void)
{
	for (uint16_t i = 0; i < ledRawSize; ++i) {
		data[i] = (2 * 104)/3;
	}
	for (uint16_t i = ledRawSize; i < length; ++i) {
		data[i] = 0;
	}
}

void transmitData(void)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, &data[0], length);

}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  __BKPT(0);
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
   */
}
