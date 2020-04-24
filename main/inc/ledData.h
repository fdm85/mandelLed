/*
 * ledData.h
 *
 *  Created on: 13.04.2020
 *      Author: cgrue
 */

#ifndef LEDDATA_H_
#define LEDDATA_H_

#include "stdint.h"

void led_initDataRaw(void);
void led_setAllLedsToUniColors(uint8_t brightness);
void led_pasteData(void);
void led_transmitData(void);

#endif /* LEDDATA_H_ */
