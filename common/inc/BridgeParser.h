/*
 * BridgeParser.h
 *
 *  Created on: 12.12.2025
 *      Author: cgrue
 */

#ifndef MAIN_BRIDGEPARSER_H_
#define MAIN_BRIDGEPARSER_H_

#include <stdint.h>
#include "peripheral.h"

typedef enum pb_ParserState_t{
  pb_eIdle,
  pb_eBusy,
  pb_eTimeOut,
  pb_eEnq,
  pb_eAck,
}pb_ParserState;

typedef struct pb_Ctx_t {
  char *pl;/**< payload access */
  void *const hwCtx;
  const uint8_t sz; /**< size of payload field*/
  const uint32_t to; /**< timeout value */
uint8_t rd; /**< read pointer/idx */
  volatile uint8_t wr; /**< write pointer/idx */
  volatile uint32_t lastToggle;
  pb_ParserState prsrStt;
}pb_Ctx;


void bp_ResetRx(pb_Ctx *const rCtx);
void bp_ResetTx(pb_Ctx *const tCtx);
static inline void Rewind(pb_Ctx *const rCtx) {
  rCtx->lastToggle = HAL_GetTick();
  rCtx->prsrStt = pb_eBusy;
}
static inline void bp_IsrCb(pb_Ctx *const rCtx) {

  Rewind(rCtx);
  if (rCtx->wr >= rCtx->sz)
    bp_ResetRx(rCtx);
}
pb_ParserState bp_Parse(pb_Ctx *const rCtx);
uint32_t pb_Convert(pb_Ctx *const rCtx);
void bPar_GetIdcs(uint8_t *iO, uint8_t *jO, uint8_t *kO);
void pb_txPutVal(pb_Ctx *const tCtx, uint32_t val);
void pb_txPutStr(pb_Ctx *const tCtx, const char * str);
#endif /* MAIN_BRIDGEPARSER_H_ */
