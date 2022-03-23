/**
 * @file      com.c
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

#include "com.h"
#include "assrt.h"
#include "stdbool.h"
#include "animations.h"
#include "usart.h"
#ifdef STM32F404xx
#include "stm32f4xx_hal_uart.h"
#endif

#ifdef STM32F303xE
#include "stm32f3xx_hal_uart.h"
#endif

static uint16_t rxBuf[64];
static uint16_t rxSize;
static bool doParse = false;

void com_testSend(void) {
	static uint8_t str[] = "hello";
	HAL_UART_Transmit(&huart2, &str[0], sizeof(str), 100uL);
}

void com_enableRx(void) {
	doParse = false;
	rxSize = 0u;
	for (uint16_t i = 0; i < 64u; ++i) {
		rxBuf[i] = 0u;
	}
	HAL_UART_Receive_IT(&huart2, (uint8_t*) &rxBuf[0], 1u);
}



/**
 * @brief  Rx Transfer completed callbacks.
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	doParse = true;
	rxSize = huart->RxXferCount;
}
