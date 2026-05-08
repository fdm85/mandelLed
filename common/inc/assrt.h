/**
 * @file      assrt.h
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
 * @brief Assert helper
 * @defgroup Dev_Helper Development helper's
 * @{
 */

#ifndef ASSRT_H_
#define ASSRT_H_

#include "cmsis_compiler.h"
#include "com.h"
#include "stddef.h"
#include <stdbool.h>

#define BREAK 1
#define TEXT 2
#define ASSRT BREAK

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE_NAME__ " : " S2(__LINE__)

#if (ASSRT == BREAK)
   #define assrt(b) if((b)==false) __BKPT(0)
   #define assrtRet(b) if((b)==false) return
#endif
#if (ASSRT == TEXT)
  #define assrt(b) if((b)==false) (com_TxBuff( LOCATION , sizeof(LOCATION)))
   #define assrtRet(b) if((b)==false) return
#endif
#ifndef ASSRT
   #define assrt(b)
   #define assrtRet(b)
#endif

#endif /* ASSRT_H_ */
/** @}*/
