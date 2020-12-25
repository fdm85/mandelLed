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


#ifdef STM32F407xx
#define PWM_TIM		htim3
#define PWM_CHAN	TIM_CHANNEL_3
#define PWM_RAW		104uL
#endif

#ifdef STM32L476xx
#define PWM_TIM		htim4
#define PWM_CHAN	TIM_CHANNEL_1
#define PWM_RAW		101uL
#endif

#ifdef STM32F303xE
#define PWM_TIM		htim1
#define PWM_CHAN	TIM_CHANNEL_4
#endif


#define ledRawSize (3u * 8u)
#define resLength 41u
#define lRawTotalCount (ledRawSize * D_LED_COUNT)
#define lRawOn ((2uL * PWM_RAW)/3uL)
#define lRawOff ((1uL * PWM_RAW)/3uL)
#define lRawTotalLength ((41 * 2) + (D_LED_COUNT * 24))

typedef struct LedRaw
{
	uint32_t g[8];
	uint32_t r[8];
	uint32_t b[8];
} LedRaw;

typedef struct Lf
{
	uint32_t rI[resLength];
	LedRaw ledRaw[D_LED_COUNT];
	uint32_t rO[resLength];
} Lf;

static Led_Led_t __attribute__((section (".ccmram"))) leds[D_LED_COUNT];
static Lf f1;
static uint8_t btMult = 1u;
static uint8_t btDiv = 1u;

const uint32_t led_count = D_LED_COUNT;

void led_initDataRaw(void)
{
	f1.rI[0] = 0uL;
	f1.rO[0] = 0uL;
	for (uint16_t i = 1; i < resLength; ++i)
	{
		f1.rI[i] = 0;
		f1.rO[i] = 0;
	}
	led_setAllLedsToUniColors(0u);
	led_pasteData();
}

void led_setBrightnessTruncation(uint8_t mult, uint8_t div)
{
	btMult = mult;
	btDiv = div;
}

static void led_setLedColors(Led_Led_t *led, uint8_t r, uint8_t g, uint8_t b)
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

/// raw bits are stored MSB first, order is green, red, blue
static void led_convertLed(Led_Led_t *l, LedRaw *r)
{
	for (uint8_t i = 0; i < 8u; ++i)
	{
		r->g[i] = (l->g & (0x80u >> i)) ? lRawOn : lRawOff;
		r->r[i] = (l->r & (0x80u >> i)) ? lRawOn : lRawOff;
		r->b[i] = (l->b & (0x80u >> i)) ? lRawOn : lRawOff;
	}
}

void led_setLedToColor(uint32_t i, uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t iM = (uint16_t) (i % D_LED_COUNT);
	led_setLedColors(&leds[iM], r, g, b);
}

void led_getLedColor(uint32_t i, Led_Led_t *l)
{
	assrt(l);
	uint16_t iM = (uint8_t) (i % D_LED_COUNT);
	l->b = leds[iM].b;
	l->g = leds[iM].g;
	l->r = leds[iM].r;
}

void led_setAllLedsToColor(uint8_t r, uint8_t g, uint8_t b)
{
	for (uint16_t i = 0; i < D_LED_COUNT; ++i)
	{
		led_setLedColors(&leds[i], r, g, b);
	}
}

void led_setAllLedsToUniColors(uint8_t brightness)
{
	for (uint16_t i = 0; i < D_LED_COUNT; ++i)
	{
		led_setLedColors(&leds[i], brightness, brightness, brightness);
	}
}

void led_pasteData(void)
{
	for (uint16_t i = 0; i < D_LED_COUNT; ++i)
	{
		led_convertLed(&leds[i], &f1.ledRaw[i]);
	}
}

void led_transmitData(void)
{
	volatile HAL_StatusTypeDef result;
	result = HAL_TIM_PWM_Start_DMA(&PWM_TIM, PWM_CHAN, &f1.rI[0], lRawTotalLength);
	assrt(result == HAL_OK);
	(void) result;
}

