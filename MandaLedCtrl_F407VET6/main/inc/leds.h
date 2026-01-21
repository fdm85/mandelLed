/*
 * leds.h
 *
 *  Created on: 27.03.2021
 *      Author: cgrue
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "ledData.h"

#define MAIN_L_LDCNT  32uL
#define MAIN_R_LDCNT  32uL
#define MRTX_LDCNT  256uL
#define RAW_LDCNT  16uL
#define RAW_DMA_CNT (3uL*8uL*RAW_LDCNT)

#define PWM_RAW   105uL
#define lRawOn (((2uL * PWM_RAW)/3uL))
#define lRawOff (((1uL * PWM_RAW)/3uL))

extern LedChainDesc_t lcd_matrix;
extern LedChainDesc_t lcd_mainL;
extern LedChainDesc_t lcd_mainR;
#endif /* LEDS_H_ */
/** @}*/
