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

static size_t txPos = 0uL;
static char * txPtr = txBuf;
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

void com_RstTxBuf(void)
{
  for (uint32_t i = 0; i < TX_BUF_SZ; ++i)
    txBuf[i] = 0u;
  txPos = 0uL;
  txPtr = txBuf;
}

static void com_Copy2Tx(const char * in)
{
  size_t len = strlen(in);
  if((len + txPos) < TX_BUF_SZ)
  {
    strncpy(txPtr, in, (TX_BUF_SZ - txPos));
    txPos += len;
    txPtr = &txPtr[txPos];
  }
}
/**
 * @brief Trigger TX of hello
 */
void com_Init(void) {
	com_RstTxBuf();
	com_Copy2Tx("Hello World\r");
  com_enableRx();
	com_Tx();
}
void com_Tx(void)
{
  HAL_UART_Transmit(&huart2, (const uint8_t*)txBuf, (uint16_t)txPos, 1000uL);
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
	  com_Copy2Tx("Uart RX Init failed!");
	  com_Copy2Tx("\r");
	}
	else
	  com_Copy2Tx("Uart RX Init succeeded\r");
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
