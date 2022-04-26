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
 * @brief Implements frequency driven animations
 * @defgroup FrqDrv frequency driven animations
 * @ingroup Animations
 */
#define colorSetVal 100u
#include "animations.h"
#include "assrt.h"
#define fm_frqBand(name, gf, str, len, rr, gg, bb, bnd, mx)\
	static const frqBand_t name = {.gCv = gf, .pSt = str, .pL = len, .r = rr, .g = gg, .b = bb, .band = bnd, .pM = (str + (len - 1)/2), .hL = (len - 1)/2, .max = mx}

fm_frqBand(bl0, getLChanVal2, 0u, 27u, 0u, 0u, colorSetVal, e63Hz, MAX_OUT);
fm_frqBand(br0, getRChanVal2, (619u - 39), 39u, 0u, 0u, colorSetVal, e63Hz, MAX_OUT);
fm_frqBand(bl1, getLChanVal2, 27u, 25u, colorSetVal, 0u, colorSetVal, e160Hz, MAX_OUT);
fm_frqBand(br1, getRChanVal2, (619u - 78), 39u, colorSetVal, 0u, colorSetVal, e160Hz, MAX_OUT);
fm_frqBand(bl2, getLChanVal2, 52u, 23u, colorSetVal, colorSetVal, 0u, e400Hz, MAX_OUT);
fm_frqBand(br2, getRChanVal2, (619u - 117), 39u, colorSetVal, colorSetVal, 0u, e400Hz, MAX_OUT);
fm_frqBand(bl3, getLChanVal2, 75u, 21u, 0u, colorSetVal, 0u, e1kHz, MAX_OUT);
fm_frqBand(br3, getRChanVal2, (619u - 156), 39u, 0u, colorSetVal, 0u, e1kHz, MAX_OUT);
fm_frqBand(bl4, getLChanVal2, 96u, 21u, 0u, 0u, colorSetVal, e2_5kHz, MAX_OUT);
fm_frqBand(br4, getRChanVal2, (619u - 195), 39u, 0u, 0u, colorSetVal, e2_5kHz, MAX_OUT);
fm_frqBand(bl5, getLChanVal2, 117u, 19u, colorSetVal, colorSetVal, 0u, e6_25kHz, MAX_OUT);
fm_frqBand(br5, getRChanVal2, (619u - 234), 39u, colorSetVal, colorSetVal, 0u, e6_25kHz, MAX_OUT);
fm_frqBand(bl6, getLChanVal2, 136u, 17u, colorSetVal, 0u, 0u, e16kHz, MAX_OUT);
fm_frqBand(br6, getRChanVal2, (619u - 273), 39u, colorSetVal, 0u, 0u, e16kHz, MAX_OUT);
fm_frqBand(rl0, getLChanVal2, 153u, 41u, 0u, 0u, colorSetVal, e63Hz, MAX_OUT);
fm_frqBand(rl1, getLChanVal2, 194u, 56u, colorSetVal, 0u, colorSetVal, e160Hz, MAX_OUT);
fm_frqBand(rr0, getRChanVal2, 297u, 52u, 0u, 0u, colorSetVal, e63Hz, MAX_OUT);
fm_frqBand(rr1, getRChanVal2, 256u, 41u, colorSetVal, 0u, colorSetVal, e160Hz, MAX_OUT);

const frqBand_t *frqB[] = { &bl0, &br0, &bl1, &br1, &bl2, &br2, &bl3, &br3, &bl4, &br4, &bl5, &br5, &bl6, &br6, NULL };
const frqBand_t *frqR[] = { &rl0, &rr0, &rl1, &rr1, NULL };

void anim_frqFrvRem(LedChainDesc_t *const lcd, const frqBand_t *fB) {
   uint32_t max = fB->max;
   uint32_t round = max / 2uL;

   uint32_t val = fB->gCv(fB->band);
   uint32_t scaled = (((fB->hL) * val) + round) / max;

//  uint32_t remainder = ((fB->hL) * val) % max;
   uint32_t remainderFull = val % max;
   assrt(remainderFull != 0uL);
//  uint8_t remR = (uint8_t) (((uint32_t) (remainder * fB->r) / max) + 1u);
//  uint8_t remG = (uint8_t) (((uint32_t) (remainder * fB->g) / max) + 1u);
//  uint8_t remB = (uint8_t) (((uint32_t) (remainder * fB->b) / max) + 1u);
   uint32_t r = (((uint32_t) (remainderFull * fB->r) / max) + 1u);
   uint32_t g = (((uint32_t) (remainderFull * fB->g) / max) + 1u);
   uint32_t b = (((uint32_t) (remainderFull * fB->b) / max) + 1u);
   assrt(r < UINT8_MAX);
   assrt(g < UINT8_MAX);
   assrt(b < UINT8_MAX);

   uint8_t remRF = (uint8_t) r;
   uint8_t remGF = (uint8_t) g;
   uint8_t remBF = (uint8_t) b;

   // blank all related
//   for (uint32_t i = 0; i < fB->pL; ++i) {
//      led_setLedToColor(lcd, fB->pSt + i, 0u, 0u, 0u);
//   }

   // blank all related
   for (uint32_t i = 0; i < fB->pL; ++i) {
      led_setLedToColor(lcd, fB->pSt + i, remRF, remGF, remBF);
   }
}

void anim_frqDrv(LedChainDesc_t *const lcd, const frqBand_t *fB) {
   uint32_t max = fB->max;
   uint32_t round = max / 2uL;

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
//   for (uint32_t i = 0; i < fB->pL; ++i) {
//      led_setLedToColor(lcd, fB->pSt + i, 0u, 0u, 0u);
//   }
//   for (uint32_t i = 0; i < fB->pL; ++i) {
//      led_setLedToColor(lcd, fB->pSt + i, 0u, 0u, 0u);
//   }

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
