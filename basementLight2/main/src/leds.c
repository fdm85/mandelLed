/**
 * @file      leds.c
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
 * @brief Allocation of memory for the LEDs
 * @ingroup LedCreator
 * @ingroup BasementLight
 * @{
 */

#include "leds.h"
#include "tim.h"

/** @brief count of on full PWM cycle */
#define PWM_RAW		104uL
/** @brief PWM set val of a set bit */
#define lRawOn ((2uL * PWM_RAW)/3uL)
/** @brief PWM set val of a reset bit */
#define lRawOff ((1uL * PWM_RAW)/3uL)

lLogicContainer(main, 619);
lRawContainer(main, 619);
lChainDesc(main, htim3, TIM_CHANNEL_3, lRawOn, lRawOff);

lLogicContainer(matrix, 256);
lRawContainer(matrix, 256);
lChainDesc(matrix, htim4, TIM_CHANNEL_2, lRawOn, lRawOff);

/** @}*/
