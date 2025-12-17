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

typedef struct pb_Ctx_t {
  char *pl;/**< payload access */
  void *const hwCtx;
  const uint8_t sz; /**< size of payload field*/
  const uint32_t to; /**< timeout value */
  uint8_t rd; /**< read pointer/idx */
  volatile uint8_t wr; /**< write pointer/idx */
  volatile uint32_t lastToggle;
}pb_Ctx;

typedef enum pb_ParserState_t{
  pb_eIdle,
  pb_eBusy,
  pb_eTimeOut,
  pb_eEnq,
  pb_eAck,
}pb_ParserState;
void bp_Reset(pb_Ctx *const ctx);
static inline void Rewind(pb_Ctx *const ctx) {
  ctx->lastToggle = HAL_GetTick();
}
static inline void bp_IsrCb(pb_Ctx *const ctx) {

  Rewind(ctx);
  if (ctx->wr >= ctx->sz)
    bp_Reset(ctx);
}
pb_ParserState bp_Parse(pb_Ctx *const ctx);
uint32_t pb_Convert(pb_Ctx *const ctx);


#endif /* MAIN_BRIDGEPARSER_H_ */
