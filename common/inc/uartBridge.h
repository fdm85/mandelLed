/**
 * @file      uartBridge.h.h
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
 * @brief UART Bridge interface
 * @defgroup uartBridge Public Interface and type declarations.
 * @{
 */

#ifndef INC_UARTBRIDGE_H_
#define INC_UARTBRIDGE_H_

#include "animations.h"

typedef void(*gtParam)(mAnim_t *const ctx, const uint32_t * params);
typedef struct uBrdg_Leaf_t{
  gtParam gtFp; /*!< set function */
  const char * des; /*!< descriptor text */
  void * par; /*!< parameter array */
  uint8_t pCt; /*!< size of parameter array */
}uBrdg_Leaf;

#endif /* INC_UARTBRIDGE_H_ */
/** @}*/
