/*
 * ledData.h
 *
 *  Created on: 13.04.2020
 *      Author: cgrue
 */

#ifndef LEDDATA_H_
#define LEDDATA_H_

#include "stdint.h"
#include "IQmathLib.h"
extern const uint32_t led_count;


typedef struct Led_Led{
	uint8_t g;
	uint8_t r;
	uint8_t b;
}Led_Led_t;

typedef struct Led_diffColor{
	_iq g;
	_iq r;
	_iq b;
	_iq gP;
	_iq rP;
	_iq bP;
	uint16_t itCur;
	uint16_t itMax;
}Led_progColor_t;


void led_initDataRaw(void);
void led_setAllLedsToUniColors(uint8_t brightness);
void led_setLedToColor(uint32_t i, uint8_t r, uint8_t g, uint8_t b);
void led_getLedColor(uint32_t i, Led_Led_t *l);
void led_pasteData(void);
void led_transmitData(void);
void led_setBrightnessTruncation(uint32_t mult, uint32_t div);
void led_setAllLedsToColor(uint8_t r, uint8_t g, uint8_t b);
uint32_t getLedCount(void);

#endif /* LEDDATA_H_ */
