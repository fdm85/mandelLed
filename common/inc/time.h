/**
 * @file      time.h
 * @authors   Clemens Grünberger
 * @copyright 2022  Clemens Grünberger
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @brief Timer implementation
 * @defgroup Timer Simple time management
 * @ingroup Dev_Help
 */

#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>
#ifdef STM32F303xE
#include "stm32f3xx_hal.h"
#endif

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
