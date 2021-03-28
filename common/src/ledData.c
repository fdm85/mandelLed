/*
 * ledData.c
 *
 *  Created on: 13.04.2020
 *      Author: cgrue
 */

#include "ledData.h"
#include "stdint.h"
#include "assrt.h"

#include "tim.h"


uint32_t getLedCount(LedChainDesc_t* lcd)
{
	return lcd->lRaw->ledCount;
}

/// raw bits are stored MSB first, order is green, red, blue
static void led_convertLed(LedChainDesc_t* lcd, LedLogic_t *l, LedRaw *r)
{
	for (uint8_t i = 0; i < 8u; ++i)
	{
		r->g[i] = (l->g & (0x80u >> i)) ? lcd->rawOn : lcd->rawOff;
		r->r[i] = (l->r & (0x80u >> i)) ? lcd->rawOn : lcd->rawOff;
		r->b[i] = (l->b & (0x80u >> i)) ? lcd->rawOn : lcd->rawOff;
	}
}

void led_initDataRaw(LedChainDesc_t* lcd)
{
	lcd->lRaw->rI[0] = 0uL;
	lcd->lRaw->rO[0] = 0uL;
	LedLogic_t init = {12u, 0u, 0u};
	led_convertLed(lcd, &init, lcd->lRaw->lConverterLed);
	for (uint16_t i = 1; i < resLength; ++i)
	{
		lcd->lRaw->rI[i] = 0;
		lcd->lRaw->rO[i] = 0;
	}
	led_setAllLedsToUniColors(lcd, 0u);
	led_pasteData(lcd);
}

void led_setBrightnessTruncation(LedChainDesc_t* lcd, uint32_t mult, uint32_t div)
{
	lcd->btMult = mult;
	lcd->btDiv = div;
}

static void led_setLedColors(LedLogic_t *led, uint8_t r, uint8_t g, uint8_t b, uint32_t mult, uint32_t div)
{
	uint32_t rOut = (uint32_t) (r * mult) / div;
	uint32_t gOut = (uint32_t) (g * mult) / div;
	uint32_t bOut = (uint32_t) (b * mult) / div;

	assrt(rOut <= UINT8_MAX);
	assrt(gOut <= UINT8_MAX);
	assrt(bOut <= UINT8_MAX);
	led->r = (uint8_t) rOut;
	led->g = (uint8_t) gOut;
	led->b = (uint8_t) bOut;
}

void led_setLedToColor(LedChainDesc_t* lcd, uint32_t i, uint8_t r, uint8_t g, uint8_t b)
{
	if(i > lcd->lRaw->ledCount)
		assrt(false);
	led_setLedColors(&lcd->lLogic[i], r, g, b, lcd->btMult, lcd->btDiv);
}

void led_getLedColor(LedChainDesc_t* lcd, uint32_t i, LedLogic_t *l)
{
	assrt(l);
	if(i > lcd->lRaw->ledCount)
		assrt(false);
	l->b = lcd->lLogic[i].b;
	l->g = lcd->lLogic[i].g;
	l->r = lcd->lLogic[i].r;
}

void led_setAllLedsToColor(LedChainDesc_t* lcd, uint8_t r, uint8_t g, uint8_t b)
{
	for (uint16_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{
		led_setLedColors(&lcd->lLogic[i], r, g, b, lcd->btMult, lcd->btDiv);
	}
}

void led_setAllLedsToUniColors(LedChainDesc_t* lcd, uint8_t brightness)
{
	for (uint16_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{
		led_setLedColors(&lcd->lLogic[i], brightness, brightness, brightness, lcd->btMult, lcd->btDiv);
	}
}

void led_pasteData(LedChainDesc_t* lcd)
{
	for (uint16_t i = 0; i < lcd->lRaw->ledCount; ++i)
	{
		led_convertLed(lcd, &lcd->lLogic[i], &lcd->lRaw->lRaw[i]);
	}
}

void led_transmitData(LedChainDesc_t* lcd)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Start_DMA(lcd->timer, lcd->timChannel, &lcd->lRaw->rI[0], lcd->lRaw->txCountInUi32);
	assrt(result == HAL_OK);
	(void) result;
}

