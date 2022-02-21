/**
 * @file      frqDrv.c
 * @authors   Clemens Grünberger
 * @copyright 2022  Clemens Grünberger
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @brief // ToDo
 * @defgroup // ToDo
 * @ingroup // ToDo
 */
#define colorSetVal 100u
#include "animations.h"

#define fm_frqBand(name, gf, str, len, rr, gg, bb, bnd)\
	static const frqBand_t name = {.gCv = gf, .pSt = str, .pL = len, .r = rr, .g = gg, .b = bb, .band = bnd, .pM = (str + (len - 1)/2), .hL = (len - 1)/2}

fm_frqBand(bl0, getLChanVal, 0u, 27u, 0u, 0u, colorSetVal, e63Hz);
fm_frqBand(br0, getRChanVal, (619u - 39), 39u, 0u, 0u, colorSetVal, e63Hz);
fm_frqBand(bl1, getLChanVal, 27u, 25u, colorSetVal, 0u, colorSetVal, e160Hz);
fm_frqBand(br1, getRChanVal, (619u - 78), 39u, colorSetVal, 0u, colorSetVal, e160Hz);
fm_frqBand(bl2, getLChanVal, 52u, 23u, colorSetVal, colorSetVal, 0u, e400Hz);
fm_frqBand(br2, getRChanVal, (619u - 117), 39u, colorSetVal, colorSetVal, 0u, e400Hz);
fm_frqBand(bl3, getLChanVal, 75u, 21u, 0u, colorSetVal, 0u, e1kHz);
fm_frqBand(br3, getRChanVal, (619u - 156), 39u, 0u, colorSetVal, 0u, e1kHz);
fm_frqBand(bl4, getLChanVal, 96u, 21u, 0u, 0u, colorSetVal, e2_5kHz);
fm_frqBand(br4, getRChanVal, (619u - 195), 39u, 0u, 0u, colorSetVal, e2_5kHz);
fm_frqBand(bl5, getLChanVal, 117u, 19u, colorSetVal, colorSetVal, 0u, e6_25kHz);
fm_frqBand(br5, getRChanVal, (619u - 234), 39u, colorSetVal, colorSetVal, 0u, e6_25kHz);
fm_frqBand(bl6, getLChanVal, 136u, 17u, colorSetVal, 0u, 0u, e16kHz);
fm_frqBand(br6, getRChanVal, (619u - 273), 39u, colorSetVal, 0u, 0u, e16kHz);

const frqBand_t *frqB[] = { &bl0, &br0, &bl1, &br1, &bl2, &br2, &bl3, &br3, &bl4, &br4, &bl5, &br5, &bl6, &br6, NULL };

void anim_frqDrv(LedChainDesc_t *const lcd, const frqBand_t *fB) {
   static const uint32_t max = MAX_OUT;
   static const uint32_t round = max / 2uL;

   uint32_t val = fB->gCv(fB->band);
   uint32_t scaled = (((fB->hL) * val) + round) / max;

//	uint32_t remainder = ((fB->hL) * val) % max;
   uint32_t remainderFull = val % max;
//	uint8_t remR = (uint8_t) (((uint32_t) (remainder * fB->r) / max) + 1u);
//	uint8_t remG = (uint8_t) (((uint32_t) (remainder * fB->g) / max) + 1u);
//	uint8_t remB = (uint8_t) (((uint32_t) (remainder * fB->b) / max) + 1u);
   uint8_t remRF = (uint8_t) (((uint32_t) (remainderFull * fB->r) / max) + 1u);
   uint8_t remGF = (uint8_t) (((uint32_t) (remainderFull * fB->g) / max) + 1u);
   uint8_t remBF = (uint8_t) (((uint32_t) (remainderFull * fB->b) / max) + 1u);

   // blank all related
   for (uint32_t i = 0; i < fB->pL; ++i) {
      led_setLedToColor(lcd, fB->pSt + i, 0u, 0u, 0u);
   }

   // paint middle led
   led_setLedToColor(lcd, fB->pM, fB->r, fB->g, fB->b);
   uint32_t i;

   // paint led's in scaled range
   for (i = 1; i < scaled; ++i) {
      led_setLedToColor(lcd, fB->pM + i, fB->r, fB->g, fB->b);
      led_setLedToColor(lcd, fB->pM - i, fB->r, fB->g, fB->b);
   }

   // paint remainding
   for (; i < scaled; ++i) {
      led_setLedToColor(lcd, fB->pM + i, remRF, remGF, remBF);
      led_setLedToColor(lcd, fB->pM - i, remRF, remGF, remBF);
   }

//	// paint adjacent to in-scaled led's
//	if (scaled < fB->hL) {
//		led_setLedToColor(lcd, fB->pM + i, remRF, remGF, remBF);
//		led_setLedToColor(lcd, fB->pM - i, remRF, remGF, remBF);
//	}

// paint remainder led's

}
