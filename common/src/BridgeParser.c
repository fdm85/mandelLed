/*
 * BridgeParser.c
 *
 *  Created on: 12.12.2025
 *      Author: cgrue
 */

#include "BridgeParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "usart.h"

static uint8_t i, j, k;

void bPar_Init(void){
  i = j = k = 0u;
}

void bPar_SetIdcs(uint8_t iO, uint8_t jO, uint8_t kO){
  i = iO;
  j = jO;
  k = kO;
}

void bPar_GetIdcs(uint8_t *iO, uint8_t *jO, uint8_t *kO){
  *iO = i;
  *jO = j;
  *kO = k;
}


/**
 * @fn uint8_t pb_IsComplete(pb_Ctx* const)
 * @brief
 * c: ?1.2/r
 * s: !300/r
 *
 * c: !1.3/t3/t5/r
 * s: !1.3/t3/t5/r
 *
 * @param ctx
 * @return
 */
static uint8_t pb_IsComplete(pb_Ctx *const rCtx) {
  uint8_t res = 0u;
  for (uint8_t i = rCtx->rd; i < rCtx->wr; ++i) {
    if(!res && (rCtx->pl[i] == '!' || rCtx->pl[i] == '?')){
      rCtx->rd = i;
      ++res;
    } 
    if((res == 1u) && (rCtx))
      ++res;
  }
  return (res == 2) ? 1u : 0u;
}


static inline pb_ParserState Eval(pb_Ctx *const rCtx) {
  pb_ParserState res = rCtx->prsrStt;
  uint8_t len = rCtx->wr - rCtx->rd;
  if((len > 3u) && pb_IsComplete(rCtx)) {
      res = (rCtx->pl[rCtx->rd] == '?') ? pb_eEnq : pb_eAck;
      ++rCtx->rd;
  }

  return res;
}

void bp_ResetRx(pb_Ctx *const rCtx){
  for (uint16_t i = 0u; i < rCtx->sz; ++i)
    rCtx->pl[i] = 0u;
  rCtx->rd = rCtx->wr = 0u;
  ((UART_HandleTypeDef*)rCtx->hwCtx)->pRxBuffPtr = (uint8_t*)rCtx->pl;
  rCtx->prsrStt = pb_eIdle;
}

void bp_ResetTx(pb_Ctx *const tCtx){
  for (uint16_t i = 0u; i < tCtx->sz; ++i)
    tCtx->pl[i] = 0u;
  tCtx->rd = tCtx->wr = 0u;
  ((UART_HandleTypeDef*)tCtx->hwCtx)->pTxBuffPtr = (uint8_t*)tCtx->pl;
  tCtx->prsrStt = pb_eIdle;
}

pb_ParserState bp_Parse(pb_Ctx *const rCtx) {
  uint32_t aux;
  uint8_t i, j, k;

  if(rCtx->rd && (rCtx->rd == rCtx->wr))
    bp_ResetRx(rCtx);

  if(rCtx->prsrStt == pb_eIdle)
    return pb_eIdle;

  if(((HAL_GetTick() - rCtx->lastToggle) > rCtx->to)){
    bp_ResetRx(rCtx);
    return pb_eTimeOut;
  }
  else
    rCtx->prsrStt = Eval(rCtx);
  
  if(rCtx->prsrStt > pb_eTimeOut)
  {
    bPar_Init();
    aux = pb_Convert(rCtx);
    i = (aux != ULONG_MAX) ? (0xffuL & aux) : 0u;
    aux = pb_Convert(rCtx);
    j = (aux != ULONG_MAX) ? (0xffuL & aux) : 0u;
    aux = pb_Convert(rCtx);
    k = (aux != ULONG_MAX) ? (0xffuL & aux) : 0u;
    bPar_SetIdcs(i, j, k);
  }

  return rCtx->prsrStt;
}

uint32_t pb_Convert(pb_Ctx *const rCtx) {
  char *rdO = &rCtx->pl[rCtx->rd], *rdN;
  uint32_t res = strtoul(&rCtx->pl[rCtx->rd], &rdN, 10);
  int32_t dlt = (rdN - rdO);
  
  if(dlt >= 0){
    rCtx->rd += (uint8_t)dlt;
    if((rCtx->pl[rCtx->rd] != '\r') && ((rCtx->pl[rCtx->rd] < '0') || (rCtx->pl[rCtx->rd] > '9')))
      ++rCtx->rd;
  }
  return res;
}


void pb_txPutIndices(pb_Ctx *const tCtx, uint8_t i, uint8_t j, uint8_t k){
  int32_t dlt = snprintf(&tCtx->pl[tCtx->wr], tCtx->sz - tCtx->wr, "!%u", i);
  if (dlt >= 0)
    tCtx->wr += (uint8_t) dlt;
  if (j) {
    dlt = snprintf(&tCtx->pl[tCtx->wr], tCtx->sz - tCtx->wr, ".%u", j);
    if (dlt >= 0)
      tCtx->wr += (uint8_t) dlt;
  }
  if (k) {
    dlt = snprintf(&tCtx->pl[tCtx->wr], tCtx->sz - tCtx->wr, ".%u", k);
    if (dlt >= 0)
      tCtx->wr += (uint8_t) dlt;
  }
}

void pb_txPutVal(pb_Ctx *const tCtx, uint32_t val) {

  int32_t dlt = snprintf(&tCtx->pl[tCtx->wr], tCtx->sz - tCtx->wr, "\t%lu", val);

  if(dlt >= 0)
    tCtx->wr += (uint8_t)dlt;
}

void pb_txPutStr(pb_Ctx *const tCtx, const char * str) {

  int32_t dlt = snprintf(&tCtx->pl[tCtx->wr], tCtx->sz - tCtx->wr, "%s", str);

  if(dlt >= 0)
    tCtx->wr += (uint8_t)dlt;
}
   
