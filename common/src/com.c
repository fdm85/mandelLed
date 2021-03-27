/*
 * com.c
 *
 *  Created on: 10.09.2020
 *      Author: cgrue
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

void com_parse(void) {

	if (doParse) {
		switch (rxBuf[0]) {
		case 'm':
			anim_nextMode(lcd);
			break;
		case 'h':
			anim_addBrightness(15);
			break;
		case 'd':
			anim_addBrightness(-15);
			break;

		default:
			break;
		}
		com_enableRx();
	}
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
