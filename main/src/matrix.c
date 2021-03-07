/******************************************************************************/
/*!
 * \file
 *
 * \brief info: add file brief
 *
 * todo: add file description
 ******************************************************************************/

#include "matrix.h"
#include "ledData.h"

typedef struct {

} undef;

channel_t left[channelWidth];
channel_t lright[channelWidth];

static void mtrx_InitL(void)
{
	uint8_t L0[8] = { 254u, 191u, 190u, 127u, 126u, 64u, 63u, 0u };
	uint8_t L1[8] = { 253u, 192u, 189u, 128u, 125u, 65u, 62u, 1u };
	uint8_t L2[8] = { 252u, 193u, 188u, 129u, 124u, 66u, 61u, 2u };
	uint8_t L3[8] = { 251u, 194u, 187u, 130u, 123u, 67u, 60u, 3u };
	uint8_t L4[8] = { 250u, 195u, 186u, 131u, 122u, 68u, 59u, 4u };
	uint8_t L5[8] = { 249u, 196u, 185u, 132u, 121u, 69u, 58u, 5u };

	for (uint8_t i = 0; i < barHeigth; ++i) {
		left[0].bar[0].dots[i] = L0[i];
		left[0].bar[1].dots[i] = L1[i];
		left[1].bar[0].dots[i] = L2[i];
		left[1].bar[1].dots[i] = L3[i];
		left[2].bar[0].dots[i] = L4[i];
		left[2].bar[1].dots[i] = L5[i];
	}
//	left.bar[0].dots[] = { 254u, 191u, 190u, 127u, 126u, 64u, 63u, 0u };
//	left.bar[0].dots[7] = 0u;
//	left.bar[0].dots[6] = 63u;
//	left.bar[0].dots[5] = 64u;
//	left.bar[0].dots[4] = 126u;
//	left.bar[0].dots[3] = 127u;
//	left.bar[0].dots[2] = 190u;
//	left.bar[0].dots[1] = 191u;
//	left.bar[0].dots[0] = 254u;
//
//	left.bar[1].dots[7] = 1u;
//	left.bar[1].dots[6] = 62u;
//	left.bar[1].dots[5] = 65u;
//	left.bar[1].dots[4] = 125u;
//	left.bar[1].dots[3] = 128u;
//	left.bar[1].dots[2] = 189u;
//	left.bar[1].dots[1] = 192u;
//	left.bar[1].dots[0] = 253u;
}

void mtrx_Init(void)
{
	mtrx_InitL();
}
void mtrx_setLeds(channel_t* chan, uint8_t r, uint8_t g, uint8_t b)
{
	for (uint8_t i = 0; i < barHeigth; ++i) {
		led_setLedToColor(chan->bar[0].dots[i], r, g, b);
		led_setLedToColor(chan->bar[1].dots[i], r, g, b);
	}
}
