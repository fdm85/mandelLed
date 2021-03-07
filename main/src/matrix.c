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

channel_t left;
channel_t lright;

void mtrx_Init(void)
{
//	left.bar.dots = { 254u, 192u, 191u, 128u, 127u, 064u, 063u, 000u };
	left.bar.dots[0] = 254u;
	left.bar.dots[1] = 191u;
	left.bar.dots[2] = 190u;
	left.bar.dots[3] = 127u;
	left.bar.dots[4] = 126u;
	left.bar.dots[5] = 64u;
	left.bar.dots[6] = 63u;
	left.bar.dots[7] = 0u;
}


void mtrx_setLeds(bar_t* bar, uint8_t r, uint8_t g, uint8_t b)
{
	for (uint8_t i = 0; i < barHeigth; ++i) {
		led_setLedToColor(bar->dots[i], r, g, b);
	}
}
