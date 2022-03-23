/**
 * @file      peripheral.h
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
 * @brief // ToDo
 * @defgroup // ToDo
 * @ingroup // ToDo
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
bool getModeSwitch(void);
void brightnessAdc(void);
uint32_t getAdcVal(void);
void ADC_ConvCpltCallback(void* hadc);
#endif /* PERIPHERAL_H_ */
