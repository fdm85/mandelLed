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
	uint32_t duration;
}time_t;


static inline void tStart(time_t* t) {
	t->started = HAL_GetTick();
	if(!t->started)
		++t->started;
}

static inline void tReset(time_t* t) {
		t->started = 0uL;
}

static inline uint32_t tElapsed(time_t* t) {
	return (t->started ? (HAL_GetTick() - t->started) : 0uL);
}

static inline void tStop(time_t* t) {
	t->duration = tElapsed(t);
}
#endif /* TIME_H_ */
