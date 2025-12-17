/*
 * BridgeParser.c
 *
 *  Created on: 12.12.2025
 *      Author: cgrue
 */

#include "BridgeParser.h"
#include <stdlib.h>
#include <limits.h>

static uint8_t i, j, k;

void bPar_Init(void){
  i = j = k = 0u;
}

void bPar_SetIdcs(uint8_t iO, uint8_t jO, uint8_t kO){
  i = iO;
  j = jO;
  k = kO;
}


static inline void Rewind(pb_Ctx *const ctx) {
    ctx->tc = ctx->to;
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
  pb_ParserState res = pb_eIdle;

  if(pb_IsComplete(ctx)) {
      res = (ctx->pl[ctx->rd] == '?') ? pb_eEnq : pb_eAck;
  }

  return res;
}

void bp_Reset(pb_Ctx *const ctx){
  for (uint16_t i = 0u; i < ctx->sz; ++i) 
    ctx->pl[i] = 0u;
  
  ctx->rd = ctx->wr = 0u;
}
void bp_Init(pb_Ctx *const ctx, uint8_t to, uint16_t size) {
  *((uint16_t*)&ctx->sz) = size;
  *((uint8_t*)&ctx->to) = to;
  ctx->pl = (char *) malloc(ctx->sz);
  
  bp_Reset(ctx);
}

void bp_Fill(pb_Ctx *const ctx, uint16_t inc) { 
  if(inc){
    ctx->wr += 0xffu & inc;
    if(ctx->wr >= ctx->sz){
      bp_Reset(ctx);
    }
    
    Rewind(ctx);
  } 
}

pb_ParserState bp_Parse(pb_Ctx *const ctx) { 
  pb_ParserState res = pb_eIdle;
  uint32_t aux;
  uint8_t i, j, k;
  if(ctx->tc == 1){
    bp_Reset(ctx);
    res = pb_eTimeOut;
  }
  else{
    --ctx->tc;
    res = Eval(ctx);
  }
  
  if(res > pb_eTimeOut)
  {
    aux = pb_Convert(ctx);
    i = (aux != ULONG_MAX) ? (0xffuL & aux) : 0u;
    aux = pb_Convert(ctx);
    j = (aux != ULONG_MAX) ? (0xffuL & aux) : 0u;
    aux = pb_Convert(ctx);
    k = (aux != ULONG_MAX) ? (0xffuL & aux) : 0u;
    bPar_SetIdcs(i, j, k);
  }

  if(ctx->rd && (ctx->rd == ctx->wr))
    bp_Reset(ctx);
  return res;
}

uint32_t pb_Convert(pb_Ctx *const ctx) {
  char *rdO = &ctx->pl[ctx->rd], *rdN;
  uint32_t res = strtoul(&ctx->pl[ctx->rd], &rdN, 10);
  int32_t dlt = (rdN - rdO);
  
  if(dlt >= 0)
    ctx->rd += (uint8_t)dlt;
  return res;
}
   
