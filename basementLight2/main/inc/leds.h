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


#define PWM_RAW   27uL
#define lRawOn (((2uL * PWM_RAW)/3uL) + 2)
#define lRawOff (((1uL * PWM_RAW)/3uL) + 1)

extern lRawDma_t matrix_dma;
extern LedChainDesc_t lcd_matrix;
extern LedChainDesc_t lcd_mainL;
extern LedChainDesc_t lcd_mainR;
#endif /* LEDS_H_ */
/** @}*/
