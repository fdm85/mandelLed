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



static uint32_t btMult = 1u;
static uint32_t btDiv = 1u;

uint32_t getLedCount(const LedChainDesc_t* lcd)
{
	return lcd->lRaw->ledCount;
}

/// raw bits are stored MSB first, order is green, red, blue
static void led_convertLed(const LedChainDesc_t* lcd, LedLogic_t *l, LedRaw *r)
{
	for (uint8_t i = 0; i < 8u; ++i)
	{
		r->g[i] = (l->g & (0x80u >> i)) ? lcd->rawOn : lcd->rawOff;
		r->r[i] = (l->r & (0x80u >> i)) ? lcd->rawOn : lcd->rawOff;
		r->b[i] = (l->b & (0x80u >> i)) ? lcd->rawOn : lcd->rawOff;
	}
}

void led_initDataRaw(const LedChainDesc_t* lcd)
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

void led_setBrightnessTruncation(const LedChainDesc_t* lcd, uint32_t mult, uint32_t div)
{
	btMult = mult;
	btDiv = div;
}

static void led_setLedColors(LedLogic_t *led, uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t rOut = (uint32_t) (r * btMult) / btDiv;
	uint32_t gOut = (uint32_t) (g * btMult) / btDiv;
	uint32_t bOut = (uint32_t) (b * btMult) / btDiv;

	assrt(rOut <= UINT8_MAX);
	assrt(gOut <= UINT8_MAX);
	assrt(bOut <= UINT8_MAX);
	led->r = (uint8_t) rOut;
	led->g = (uint8_t) gOut;
	led->b = (uint8_t) bOut;
}

void led_setLedToColor(const LedChainDesc_t* lcd, uint32_t i, uint8_t r, uint8_t g, uint8_t b)
{
	if(i > lcd->lRaw->ledCount)
		assrt(false);
	led_setLedColors(&lcd->lLogic[i], r, g, b);
}

void led_getLedColor(const LedChainDesc_t* lcd, uint32_t i, LedLogic_t *l)
{
	assrt(l);
	if(i > lcd->lRaw->ledCount)
		assrt(false);
	l->b = lcd->lLogic[i].b;
	l->g = lcd->lLogic[i].g;
	l->r = lcd->lLogic[i].r;
}

void led_setAllLedsToColor(const LedChainDesc_t* lcd, uint8_t r, uint8_t g, uint8_t b)
{
	for (uint16_t i = 0; i < D_LED_COUNT; ++i)
	{
		led_setLedColors(&lcd->lLogic[i], r, g, b);
	}
}

void led_setAllLedsToUniColors(const LedChainDesc_t* lcd, uint8_t brightness)
{
	for (uint16_t i = 0; i < D_LED_COUNT; ++i)
	{
		led_setLedColors(&lcd->lLogic[i], brightness, brightness, brightness);
	}
}

void led_pasteData(const LedChainDesc_t* lcd)
{
	for (uint16_t i = 0; i < D_LED_COUNT; ++i)
	{
		led_convertLed(lcd, &lcd->lLogic[i], &lcd->lRaw->lRaw[i]);
	}
}

void led_transmitData(const LedChainDesc_t* lcd)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Start_DMA(lcd->timer, lcd->timChannel, &lcd->lRaw->rI[0], lcd->lRaw->txCountInUi32);
	assrt(result == HAL_OK);
	(void) result;
}

