/*
 * matrix.h
 *
 *  Created on: 07.03.2021
 *      Author: cgrue
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#define barHeigth 8
#define barWidth 2
#define channelWidth 7
#define auxChanWidth 1

#include <stdint.h>
#include "ledData.h"
#include "animations.h"

void mtrx_Init(void);
void mtrx_anim(mAnim_t* ctx);
#endif /* MATRIX_H_ */
