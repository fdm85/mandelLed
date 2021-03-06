/*
 * time.h
 *
 *  Created on: 06.03.2021
 *      Author: cgrue
 */

#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>
#include "stm32f3xx_hal.h"

typedef struct time_s
{
	uint32_t started;
}time_t;


static inline void tStart(time_t* t) {
	t->started = HAL_GetTick();
}

static inline uint32_t tElapsed(time_t* t) {
	return (HAL_GetTick() - t->started);
}
#endif /* TIME_H_ */
