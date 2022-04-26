/**
 * @file      leds.h
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
 * @brief Export of allocated led strip context's
 * @defgroup LedCreator Allocates memory for the LEDs
 * @ingroup BasementLight
 */

#ifndef LEDS_H_
#define LEDS_H_



#include "ledData.h"

extern LedChainDesc_t lcd_main;
extern LedChainDesc_t lcd_matrix;
#endif /* LEDS_H_ */
