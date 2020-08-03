/*
 * ledData.h
 *
 *  Created on: 13.04.2020
 *      Author: cgrue
 */

#ifndef LEDDATA_H_
#define LEDDATA_H_

#include "stdint.h"
extern const uint32_t led_count;

typedef struct Led_Led{
	uint8_t g;
	uint8_t r;
	uint8_t b;
}Led_Led_t;

typedef struct Led_diffColor{
	int16_t g;
	int16_t r;
	int16_t b;
}Led_diffColor_t;


void led_initDataRaw(void);
void led_setAllLedsToUniColors(uint8_t brightness);
void led_setLedToColor(uint32_t i, uint8_t r, uint8_t g, uint8_t b);
void led_getLedColor(uint32_t i, Led_Led_t *l);
void led_pasteData(void);
void led_transmitData(void);

#endif /* LEDDATA_H_ */
