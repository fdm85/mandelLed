/*
 * ledData.c
 *
 *  Created on: 13.04.2020
 *      Author: cgrue
 */

#include "ledData.h"
#include "stdint.h"
#include "tim.h"
#define ledRawSize (3u * 8u)
#define ledCount 60u
#define resLength 41u

#define lRawTotalCount (ledRawSize * ledCount)
#define lRawOn ((2uL * 104uL)/3uL)
#define lRawOff ((1uL * 104uL)/3uL)
#define lRawTotalLength ((41 * 2) + (ledCount * 24))

typedef struct Led{
	uint8_t g;
	uint8_t r;
	uint8_t b;
}Led;

typedef struct LedRaw{
	uint32_t g[8];
	uint32_t r[8];
	uint32_t b[8];
}LedRaw;

typedef struct Lf {
	uint32_t rI[resLength];
	LedRaw ledRaw[ledCount];
	uint32_t rO[resLength];
}Lf;

static Led leds[ledCount];
static Lf f1;

void led_initDataRaw(void)
{
	f1.rI[0] = lRawOff;
	f1.rO[0] = 0uL;
	for (uint16_t i = 1; i < resLength; ++i) {
			f1.rI[i] = 0;
			f1.rO[i] = 0;
	}
	led_setAllLedsToUniColors(0u);
	led_pasteData();
}


static void led_setLedColors(Led* led, uint8_t r, uint8_t g, uint8_t b)
{
	led->r = r;
	led->g = g;
	led->b = b;
}

/// raw bits are stored MSB first, order is green, red, blue
static void led_convertLed(Led* l, LedRaw* r)
{
	for (uint8_t i = 0; i < 8u; ++i) {
		r->g[i] = (l->g & (0x80u >> i)) ? lRawOn : lRawOff;
		r->r[i] = (l->r & (0x80u >> i)) ? lRawOn : lRawOff;
		r->b[i] = (l->b & (0x80u >> i)) ? lRawOn : lRawOff;
	}
}

void led_setAllLedsToUniColors(uint8_t brightness)
{
	for (uint8_t i = 0; i < ledCount; ++i) {
		led_setLedColors(&leds[i], brightness, brightness, brightness);
	}
}

void led_pasteData(void)
{
	for (uint8_t i = 0; i < ledCount; ++i) {
		led_convertLed(&leds[i], &f1.ledRaw[i]);
	}
}

void led_transmitData(void)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, &f1.rI[0], lRawTotalLength);
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
