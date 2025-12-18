/*
 * BridgeParser.c
 *
 *  Created on: 12.12.2025
 *      Author: cgrue
 */

#include "BridgeParser.h"
#include "esp_log.h"
#include <stdint.h>
#include <stdlib.h>


static inline void Rewind(pb_Ctx *const ctx) {
    ctx->tc = ctx->to;
}

static uint8_t pb_IsComplete(pb_Ctx *const ctx) {
  uint8_t res = 0u;
  for (uint8_t i = ctx->rd; i < ctx->wr; ++i) {
    if(!res && (ctx->pl[i] == '!')){
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
  uint8_t len = ctx->wr - ctx->rd;

  if(len >= 3u) {
    if(pb_IsComplete(ctx))
      res = (ctx->pl[ctx->rd] == '\t') ? pb_eRxNum : pb_eRxTxt;
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
    ctx->wr += inc;
    if(ctx->wr >= ctx->sz){
      ESP_LOGW("BridgeParser", "Full r/w:%u/%d s: %s", ctx->rd, ctx->wr, ctx->pl);
      bp_Reset(ctx);
    }
    
    Rewind(ctx);
  } 
}

pb_ParserState bp_Parse(pb_Ctx *const ctx) { 
  pb_ParserState res = pb_eIdle;
  
  if(ctx->tc == 1){
    bp_Reset(ctx);
    res = pb_eTimeOut;
  }
  else{
    --ctx->tc;
    res = Eval(ctx);
  }
  
  if(ctx->rd && (ctx->rd == ctx->wr))
    bp_Reset(ctx);
  return res;
}

uint32_t pb_Convert(pb_Ctx *const ctx) {
  char *rdO = &ctx->pl[ctx->rd], *rdN;
  uint32_t res = strtoul_l(&ctx->pl[ctx->rd], &rdN, 10, NULL);
  
  ctx->rd += (rdN - rdO);
  return res;
}
   