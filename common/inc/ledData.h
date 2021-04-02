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
#include "tim.h"

// logical led container
typedef struct LedLogic_tag{
	uint8_t g;
	uint8_t r;
	uint8_t b;
}LedLogic_t;

// led color transition descriptor
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

typedef struct diffRunnerCtx_tag
{
	Led_progColor_t * lDc;
	uint32_t size;
}diffRunnerCtx_t;

// raw led data for DMA to timer transfer
typedef struct LedRaw
{
	uint32_t g[8];
	uint32_t r[8];
	uint32_t b[8];
} LedRaw;

typedef struct lRawCont_tag
{
	uint32_t* rI;
	uint32_t* rO;
	LedRaw* lConverterLed;
	LedRaw* lRaw;
	uint32_t ledCount;
	uint16_t txCountInUi32;
	uint16_t padding;
}lRawCont_t;

#define resLength 41u
#define lRawContainer(name, ledCnt) \
	static struct \
	{ \
		uint32_t rI[resLength]; \
		LedRaw converterLed[1]; \
		LedRaw ledRaw[ledCnt]; \
		uint32_t rO[resLength]; \
	} lRawContainer_##name; \
	const lRawCont_t lRawCont_##name = { \
			.rI = &lRawContainer_##name.rI[0], \
			.rO = &lRawContainer_##name.rO[0], \
			.lConverterLed = &lRawContainer_##name.converterLed[0], \
			.lRaw = &lRawContainer_##name.ledRaw[0], \
			.ledCount = ledCnt, \
			.txCountInUi32 = (sizeof(lRawContainer_##name)/sizeof(uint32_t)), \
	}

#define lLogicContainer(name, ledCnt)\
	static LedLogic_t __attribute__((section (".ccmram"))) ledsLog_##name[ledCnt]

#define lChainDesc(name, timerN, tChan, rOn, rOff) \
	LedChainDesc_t lcd_##name = { \
			.lLogic = &ledsLog_##name[0], \
			.lRaw = &lRawCont_##name, \
			.timer = &timerN, \
			.timChannel = tChan, \
			.rawOn = rOn, \
			.rawOff = rOff, \
	}

// led chain descriptor, one for each led tube/chain
typedef struct LedChainDesc_tag
{
	LedLogic_t* lLogic;
	const lRawCont_t *const lRaw;
	TIM_HandleTypeDef* timer;
	uint32_t timChannel;
	uint32_t rawOn;
	uint32_t rawOff;
	uint32_t btMult;
	uint32_t btDiv;
} LedChainDesc_t;

void led_initDataRaw(LedChainDesc_t* lcd);
void led_setAllLedsToUniColors(LedChainDesc_t* lcd, uint8_t brightness);
void led_setLedToColor(LedChainDesc_t* lcd, uint32_t i, uint8_t r, uint8_t g, uint8_t b);
void led_getLedColor(LedChainDesc_t* lcd, uint32_t i, LedLogic_t *l);
void led_pasteData(LedChainDesc_t* lcd);
void led_transmitData(LedChainDesc_t* lcd);
void led_setBrightnessTruncation(LedChainDesc_t* lcd, uint32_t mult, uint32_t div);
void led_setAllLedsToColor(LedChainDesc_t* lcd, uint8_t r, uint8_t g, uint8_t b);
uint32_t getLedCount(LedChainDesc_t* lcd);

#endif /* LEDDATA_H_ */
