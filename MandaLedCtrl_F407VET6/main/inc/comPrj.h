/*
 * ccomPrj.h
 *
 *  Created on: Jan 9, 2026
 *      Author: cgrue
 */

#ifndef INC_COMPRJ_H_
#define INC_COMPRJ_H_

#define RX_BUF_SZ 64u
#define TX_BUF_SZ 64u

#include "usart.h"
#include "BridgeParser.h"

extern char rxBuf[RX_BUF_SZ];
extern char txBuf[TX_BUF_SZ];
extern pb_Ctx rxCtx;
extern pb_Ctx txCtx;
#endif /* INC_COMPRJ_H_ */
