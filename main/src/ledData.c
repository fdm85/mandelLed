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
#define retLength 41u
#define length (retLength + ledRawSize + retLength)

static uint32_t data[length];
static uint32_t data2[length];

void initDataRaw(void)
{
	data[0] = (1 * 104)/3;
	for (uint16_t i = 1; i < retLength; ++i) {
			data[i] = 0;
	}
	for (uint16_t i = retLength; i < (retLength + ledRawSize); ++i) {
		data[i] = (2 * 104)/3;
	}
	for (uint16_t i = (retLength + ledRawSize); i < length; ++i) {
		data[i] = 0;
	}
}

void initDataRaw2(void)
{
	data2[0] = (1 * 104)/3;
	for (uint16_t i = 1; i < retLength; ++i) {
			data[i] = 0;
	}
	for (uint16_t i = retLength; i < (retLength + ledRawSize); i += 2u) {
		data2[i] = (2 * 104)/3;
		data2[i+1] = (1 * 104)/3;
	}
	for (uint16_t i = (retLength + ledRawSize); i < length; ++i) {
		data2[i] = 0;
	}
}

void transmitData(void)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, &data[0], length);
	(void)result;
}

void transmitData2(void)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, &data2[0], length);
	(void)result;

}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

//  __BKPT(0);
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
   */
}
