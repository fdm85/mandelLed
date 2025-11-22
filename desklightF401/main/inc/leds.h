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


#define PWM_RAW   90uL
#define lRawOn ((2uL * PWM_RAW)/3uL)
#define lRawOff ((1uL * PWM_RAW)/3uL)

extern lRawDma_t matrix_dma;
extern LedChainDesc_t lcd_main;
extern LedChainDesc_t lcd_matrix;
#endif /* LEDS_H_ */
