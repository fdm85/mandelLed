/*
 * matrix.h
 *
 *  Created on: 07.03.2021
 *      Author: cgrue
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#define barHeigth 8
#define channelWidth 2

#include <stdint.h>

typedef struct bar_tag {
	uint8_t dots[barHeigth];
}bar_t;

typedef struct channel_tag {
	bar_t bar;
}channel_t;

extern channel_t left;
extern channel_t lright;
void mtrx_Init(void);
void mtrx_setLeds(bar_t* bar, uint8_t r, uint8_t g, uint8_t b);

#endif /* MATRIX_H_ */
