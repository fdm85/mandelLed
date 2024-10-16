/******************************************************************************/
/*!
 * \file
 *
 * \brief info: add file brief
 *
 * todo: add file description
 ******************************************************************************/

#include "leds.h"
#include "tim.h"

#define PWM_RAW		90uL
#define lRawOn ((2uL * PWM_RAW)/3uL)
#define lRawOff ((1uL * PWM_RAW)/3uL)

lLogicContainer(main, LED_1);
//lRawContainer(main, LED_1);
lRawContainer(main, 8);
lChainDesc(main, htim4, TIM_CHANNEL_2, lRawOn, lRawOff);


lLogicContainer(matrix, 256);
//lRawContainer(matrix, 32);
//lChainDesc(matrix, htim3, TIM_CHANNEL_1, lRawOn, lRawOff);


static struct \
	{ \
		uint32_t rI[41u]; \
		LedRaw converterLed[1]; \
		LedRaw ledRaw[32]; \
		uint32_t rO[41u]; \
	} lRawContainer_matrix; \
	const lRawCont_t lRawCont_matrix = { \
			.rI = &lRawContainer_matrix.rI[0], \
			.rO = &lRawContainer_matrix.rO[0], \
			.lConverterLed = &lRawContainer_matrix.converterLed[0], \
			.lRaw = &lRawContainer_matrix.ledRaw[0], \
			.ledCount = 32, \
			.txCountInUi32 = (sizeof(lRawContainer_matrix)/sizeof(uint32_t)), \
	};


	LedChainDesc_t lcd_matrix = { \
				.lLogic = &ledsLog_matrix[0], \
				.lRaw = &lRawCont_matrix, \
				.timer = &htim3, \
				.timChannel = 0x00000000U, \
				.rawOn = ((2uL * 90uL)/3uL), \
				.rawOff = ((1uL * 90uL)/3uL), \
		};
