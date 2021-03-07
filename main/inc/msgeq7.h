/*
 * msgeq7.h
 *
 *  Created on: 06.03.2021
 *      Author: cgrue
 */

#ifndef MSGEQ7_H_
#define MSGEQ7_H_

#include "peripheral.h"

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

void msgeq_ticker(void);

#endif /* MSGEQ7_H_ */
