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
 * @brief Implements ComApi
 * @ingroup ComApi
 * @{
 */

#include "com.h"
#include <stdlib.h>
#include "assrt.h"
#include "stdio.h"
#include "animations.h"
#include "usart.h"
#include "faultHandling.h"
#ifdef STM32F404xx
#include "stm32f4xx_hal_uart.h"
#endif

#ifdef STM32F303xE
#include "stm32f3xx_hal_uart.h"
#endif


#define RX_BUF_SZ 64u
static char rxBuf[RX_BUF_SZ];
pb_Ctx rxCtx = {.pl = rxBuf, .hwCtx = &huart2, .sz = RX_BUF_SZ, .to = 5000uL};

#define TX_BUF_SZ 64u
static char txBuf[TX_BUF_SZ];
pb_Ctx txCtx = {.pl = txBuf, .hwCtx = &huart2, .sz = TX_BUF_SZ};

static char faultDumpBuffer[FAULT_HANDLING_DUMP_SIZE];
#define FailStr "snprintf failed!"
#define FailSz (sizeof(FailStr))

static void com_DumpFault(void) {
  HAL_UART_Transmit(&huart2, (const uint8_t *)faultDumpBuffer, sizeof(FAULT_HANDLING_DUMP_SIZE), 1000uL);
}
void com_SetDump(void) {
  faultHandlingSetDumpProcessor( faultDumpBuffer, com_DumpFault );
  faultHandlingSetPostFaultAction( POSTHANDLER_DEBUG );
}


/**
 * @brief Trigger TX of hello
 */
void com_Init(void) {
  bp_ResetTx(&txCtx);
  pb_txPutStr(&txCtx, "Hello World\r\n");
  com_enableRx();
	com_Tx();
}
void com_Tx(void)
{
  HAL_UART_Transmit_IT(txCtx.hwCtx, (const uint8_t*)&txCtx.pl[txCtx.rd], (uint16_t)txCtx.wr - txCtx.rd);
}
void com_TxBuff(const char * buff, size_t sz)
{
  HAL_UART_Transmit(&huart2, (const uint8_t*)buff, (uint16_t)sz, 1000uL);
}

/**
 * @brief Enable receive interrupt
 */
void com_enableRx(void) {

	HAL_StatusTypeDef res = HAL_UART_Receive_IT(&huart2, (uint8_t*) rxBuf, RX_BUF_SZ);

	if(res != HAL_OK)
	{
	  pb_txPutStr(&txCtx, "Uart RX Init failed!");
	}
	else
	  pb_txPutStr(&txCtx, "Uart RX Init succeeded!");
	pb_txPutStr(&txCtx, "\r\n");
}



void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  bp_ResetTx(&txCtx);
}

/**
 * @brief  Rx Transfer completed callbacks.
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

  rxCtx.wr = (uint8_t)(huart->pRxBuffPtr - (uint8_t*)rxBuf);
  bp_IsrCb(&rxCtx);
}

/** @}*/
