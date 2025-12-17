/*
 * BridgeParser.h
 *
 *  Created on: 12.12.2025
 *      Author: cgrue
 */

#ifndef MAIN_BRIDGEPARSER_H_
#define MAIN_BRIDGEPARSER_H_

#include <stdint.h>

typedef struct pb_Ctx_t {
  char *pl;/**< payload access */
  const uint16_t sz; /**< size of payload field*/
  const uint8_t to; /**< timeout value */
  uint8_t tc; /**< tick counter, used for timeout supervision */
  uint8_t rd; /**< read pointer/idx */
  uint8_t wr; /**< write pointer/idx */
}pb_Ctx;

typedef enum pb_ParserState_t{
  pb_eIdle,
  pb_eBusy,
  pb_eTimeOut,
  pb_eEnq,
  pb_eAck,
}pb_ParserState;
void bp_Reset(pb_Ctx *const ctx);
void bp_Init(pb_Ctx *const ctx, uint8_t to, uint16_t size);
void bp_Fill(pb_Ctx *const ctx, uint16_t inc);
pb_ParserState bp_Parse(pb_Ctx *const ctx);
uint32_t pb_Convert(pb_Ctx *const ctx);


#endif /* MAIN_BRIDGEPARSER_H_ */
