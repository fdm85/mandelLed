/*
 * BridgeParam.c
 *
 *  Created on: 17.12.2025
 *      Author: cgrue
 */

#include "BridgeParam.h"


/*.1 Strip (On/Off) [1]
  .2 trgIntervall(100us) [1]
  .3 Brightness (Mult, Div)[2]
  .4 activeLeds (start / end)[2]
  .5 fRender (+;-)[1]*/


uint32_t brdg_par[3][6][2];

static uint8_t i, j, k;

void bPar_Init(void){
  i = j = k = 0u;
}

void bPar_GetIdcs(uint8_t *iO, uint8_t *jO, uint8_t *kO)
{
  *iO = i;
  *jO = j;
  *kO = k;
}

void bPar_Fill(uint32_t var){
  
}