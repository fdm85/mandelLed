/**
 * @file      com.h
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
 * @brief Communication Interface
 * @defgroup ComApi Communication API
 * @{
 */

#ifndef COM_H_
#define COM_H_

#include <stdint.h>
#include <string.h>
#include "uartBridge.h"
#include "comPrj.h"

extern const uBrdg_Leaf *const leafs[];

void com_SetDump(void);
void com_Init(void);
void com_Tx(void);
void com_TxBuff(const char * buff, size_t sz);

void com_enableRx(void);

#endif /* COM_H_ */
/** @}*/
