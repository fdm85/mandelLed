/**
 * @file      msgeq7.h
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
 * @brief Implements msgeq function
 * @defgroup msgeq7 Audio signal analysing
 * @ingroup IO
 * @{
 */

#ifndef MSGEQ7_H_
#define MSGEQ7_H_

#include "peripheral.h"
#include <stdint.h>
/*! @brief max ADC value */
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
uint32_t getLChanVal2(msgeq7Freq freq);
uint32_t getRChanVal2(msgeq7Freq freq);
#define SUM_MAX (8 * MAX_OUT)
uint32_t getLSum(msgeq7Freq freq);
uint32_t getRSum(msgeq7Freq freq);
#endif /* MSGEQ7_H_ */
/** @}*/
