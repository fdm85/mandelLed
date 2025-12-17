/*
 * BridgeParser.c
 *
 *  Created on: 12.12.2025
 *      Author: cgrue
 */

#include "BridgeParser.h"
#include <stdlib.h>
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
static uint8_t pb_IsComplete(pb_Ctx *const ctx) {
  uint8_t res = 0u;
  for (uint8_t i = ctx->rd; i < ctx->wr; ++i) {
    if(!res && (ctx->pl[i] == '!' || ctx->pl[i] == '?')){
      ctx->rd = i;
      ++res;
    } 
    if((res == 1u) && (ctx->pl[i] == '\r'))
      ++res;
  }
  return (res == 2) ? 1u : 0u;
}


static inline pb_ParserState Eval(pb_Ctx *const ctx) {
  pb_ParserState res = ctx->prsrStt;
  uint8_t len = ctx->wr - ctx->rd;
  if((len > 3u) && pb_IsComplete(ctx)) {
      res = (ctx->pl[ctx->rd] == '?') ? pb_eEnq : pb_eAck;
      ++ctx->rd;
  }

  return res;
}

void bp_Reset(pb_Ctx *const ctx){
  for (uint16_t i = 0u; i < ctx->sz; ++i) 
    ctx->pl[i] = 0u;
  ctx->rd = ctx->wr = 0u;
  ((UART_HandleTypeDef*)ctx->hwCtx)->pRxBuffPtr = (uint8_t*)ctx->pl;
  ctx->prsrStt = pb_eIdle;
}

pb_ParserState bp_Parse(pb_Ctx *const ctx) { 
  uint32_t aux;
  uint8_t i, j, k;

  if(ctx->rd && (ctx->rd == ctx->wr))
    bp_Reset(ctx);

  if(ctx->prsrStt == pb_eIdle)
    return pb_eIdle;

  if(((HAL_GetTick() - ctx->lastToggle) > ctx->to)){
    bp_Reset(ctx);
    return pb_eTimeOut;
  }
  else
    ctx->prsrStt = Eval(ctx);
  
  if(ctx->prsrStt > pb_eTimeOut)
  {
    aux = pb_Convert(ctx);
    i = (aux != ULONG_MAX) ? (0xffuL & aux) : 0u;
    aux = pb_Convert(ctx);
    j = (aux != ULONG_MAX) ? (0xffuL & aux) : 0u;
    aux = pb_Convert(ctx);
    k = (aux != ULONG_MAX) ? (0xffuL & aux) : 0u;
    bPar_SetIdcs(i, j, k);
  }

  return ctx->prsrStt;
}

uint32_t pb_Convert(pb_Ctx *const ctx) {
  char *rdO = &ctx->pl[ctx->rd], *rdN;
  uint32_t res = strtoul(&ctx->pl[ctx->rd], &rdN, 10);
  int32_t dlt = (rdN - rdO);
  
  if(dlt >= 0){
    ctx->rd += (uint8_t)dlt;
    if((ctx->pl[ctx->rd] != '\r') && ((ctx->pl[ctx->rd] < '0') || (ctx->pl[ctx->rd] > '9')))
      ++ctx->rd;
  }
  return res;
}

uint32_t pb_putOut(pb_Ctx *const ctx, uint32_t val) {
  char *rdO = &ctx->pl[ctx->rd], *rdN;
  uint32_t res = strtoul(&ctx->pl[ctx->rd], &rdN, 10);
  int32_t dlt = (rdN - rdO);

  if(dlt >= 0){
    ctx->rd += (uint8_t)dlt;
    if((ctx->pl[ctx->rd] != '\r') && ((ctx->pl[ctx->rd] < '0') || (ctx->pl[ctx->rd] > '9')))
      ++ctx->rd;
  }
  return res;
}
   
