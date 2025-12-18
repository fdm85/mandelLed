/*
 * leds.h
 *
 *  Created on: 27.03.2021
 *      Author: cgrue
 */

#ifndef LEDS_H_
#define LEDS_H_

#define CCRAM_PLACING
#include "ledData.h"

#define MAIN_LDCNT  200uL
#define RAW_LDCNT  16uL
#define RAW_DMA_CNT (3uL*8uL*RAW_LDCNT)

#define PWM_RAW   90uL
#define lRawOn (((2uL * PWM_RAW)/3uL))
#define lRawOff (((1uL * PWM_RAW)/3uL))


extern LedChainDesc_t lcd_main;

#endif /* LEDS_H_ */
