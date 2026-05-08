/*
 * comPrj.c
 *
 *  Created on: Jan 9, 2026
 *      Author: cgrue
 */

#include "comPrj.h"

char rxBuf[RX_BUF_SZ];
char txBuf[TX_BUF_SZ];
pb_Ctx rxCtx = {.pl = rxBuf, .hwCtx = USART1, .sz = RX_BUF_SZ, .to = 5000uL};
pb_Ctx txCtx = {.pl = txBuf, .hwCtx = USART1, .sz = TX_BUF_SZ};

