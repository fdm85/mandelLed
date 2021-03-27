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

#include <stdint.h>

typedef struct bar_tag {
	uint8_t dots[barHeigth];
}bar_t;

typedef struct channel_tag {
	bar_t bar[barWidth];
}channel_t;

extern channel_t left[channelWidth];
extern channel_t lright[channelWidth];
void mtrx_Init(void);
void mtrx_setLeds(channel_t* chan, uint8_t r, uint8_t g, uint8_t b);
void mtrx_setLedsScaled(channel_t* chan, uint32_t val, uint8_t r, uint8_t g, uint8_t b);
#endif /* MATRIX_H_ */
