/*
 * BridgeParam.h
 *
 *  Created on: 17.12.2025
 *      Author: cgrue
 */

#ifndef MAIN_BRIDGEPARAM_H_
#define MAIN_BRIDGEPARAM_H_

#include <stdint.h>

void bPar_Init(void);
void bPar_Fill(uint32_t var);
void bPar_GetIdcs(uint8_t *iO, uint8_t *jO, uint8_t *kO);

#endif /* MAIN_BRIDGEPARAM_H_ */
