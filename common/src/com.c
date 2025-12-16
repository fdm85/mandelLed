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
#include "assrt.h"
#include "stdbool.h"
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
#define TX_BUF_SZ 64u
static char txBuf[TX_BUF_SZ];
char convBuf[CONV_BUF_SZ];
static size_t rxSize;
static bool doParse = false;
static size_t txPos = 0uL;
static char * txPtr = txBuf;
static char faultDumpBuffer[FAULT_HANDLING_DUMP_SIZE];
#define FailStr "snprintf failed!"
#define FailSz (sizeof(FailStr))

void com_RstConvBuff(void)
{
  for (uint32_t i = 0; i < CONV_BUF_SZ; ++i)
    convBuf[i] = 0u;
}
void com_uIntToStr(uint32_t v)
{
  static char tmp[CONV_BUF_SZ];
  uint8_t i = 0u;
  uint32_t j;
  com_RstConvBuff();
  do {
    tmp[i] = ((char)0xFFu & (v%10uL));
    v /= 10;
    ++i;
  } while (v && (i < (sizeof(tmp)/sizeof(tmp[0]))));

  for (j = 0uL; (j < i) && (j < CONV_BUF_SZ); ++j)
    convBuf[j] = tmp[i-j];
}
static void com_DumpFault(void) {
  HAL_UART_Transmit(&huart2, (const uint8_t *)faultDumpBuffer, sizeof(FAULT_HANDLING_DUMP_SIZE), 1000uL);
}
void com_SetDump(void) {
  faultHandlingSetDumpProcessor( faultDumpBuffer, com_DumpFault );
  faultHandlingSetPostFaultAction( POSTHANDLER_DEBUG );
}

static size_t com_StrLen(const char * str){
  size_t res = 0;
  while (str[res] != '0')
    ++res;

  return res;
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
	com_Copy2Tx("Hello World\n\r");
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
	doParse = false;
	rxSize = 0u;
	for (uint16_t i = 0; i < RX_BUF_SZ; ++i) {
		rxBuf[i] = 0u;
	}

//	HAL_StatusTypeDef res = HAL_UARTEx_ReceiveToIdle_IT(&huart2, (uint8_t*) rxBuf, 1);
	HAL_StatusTypeDef res = UART_Start_Receive_IT(&huart2, (uint8_t*) rxBuf, 1);

	if(res != HAL_OK)
	{
	  com_Copy2Tx("Uart RX Init failed!");
	  com_uIntToStr(res);
	  com_Copy2Tx("\n\r");
	}
	else
	  com_Copy2Tx("Uart RX Init succeeded\n\r");
}

/**
 * @brief  Rx Transfer completed callbacks.
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	doParse = true;
	rxSize = huart->RxXferCount;
	if(rxBuf[0] == '?')
	  for (uint32_t i = 0; leafs[i] != NULL; ++i) {
	    com_TxBuff(leafs[i]->des, com_StrLen(leafs[i]->des));
    }
	com_TxBuff(rxBuf, sizeof(RX_BUF_SZ));
	doParse = false;
}

/** @}*/
