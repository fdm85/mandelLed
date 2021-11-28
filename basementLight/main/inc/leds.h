/*
 * leds.h
 *
 *  Created on: 27.03.2021
 *      Author: cgrue
 */

#ifndef LEDS_H_
#define LEDS_H_


#define CCRAM_PLACING __attribute__((section (".ccmram")))
#include "ledData.h"

extern LedChainDesc_t lcd_main;
extern LedChainDesc_t lcd_matrix;
#endif /* LEDS_H_ */
