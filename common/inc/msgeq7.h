/*
 * msgeq7.h
 *
 *  Created on: 06.03.2021
 *      Author: cgrue
 */

#ifndef MSGEQ7_H_
#define MSGEQ7_H_

#include "peripheral.h"

#define MAX_OUT		3100uL

typedef enum {
	e63Hz = 0u,
	e160Hz,
	e400Hz,
	e1kHz,
	e2_5kHz,
	e6_25kHz,
	e16kHz,
	eMax
}msgeq7Freq;

typedef uint32_t (*gChanVal)(msgeq7Freq freq);

void msgeq_ticker(void);
uint32_t getLChanVal(msgeq7Freq freq);
uint32_t getRChanVal(msgeq7Freq freq);
#define SUM_MAX (8 * MAX_OUT)
uint32_t getLSum(msgeq7Freq freq);
uint32_t getRSum(msgeq7Freq freq);
#endif /* MSGEQ7_H_ */
