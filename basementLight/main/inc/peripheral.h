/*
 * peripheral.h
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#include <stdbool.h>
#include <stdint.h>

void initClock(void);
void initPeripherals(void);
void blueLedToggle(void);
void greenLedToggle(void);
void orangeLedToggle(void);
void outputEnableLvlShifter(void);
bool getModeSwitch(void);
void brightnessAdc(void);
uint32_t getAdcVal(void);
#endif /* PERIPHERAL_H_ */
