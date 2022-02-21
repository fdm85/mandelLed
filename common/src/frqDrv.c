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
#define colorSetVal 140u
#include "animations.h"

static const frqBand_t bl0 = {.gCv = getLChanVal, .pSt = 0u, .pL = 26u, .r = 1, .g = colorSetVal, .b = colorSetVal, .band = e63Hz};
static const frqBand_t br0 = {.gCv = getRChanVal, .pSt = (618u -30), .pL = 30u, .r = 1, .g =colorSetVal, .b = colorSetVal, .band = e63Hz};
static const frqBand_t bl1 = {.gCv = getLChanVal, .pSt = 26u, .pL = 26u, .r = colorSetVal, .g = 2u, .b = colorSetVal, .band = e160Hz};
static const frqBand_t br1 = {.gCv = getRChanVal, .pSt = (618u -60), .pL = 30u, .r = colorSetVal, .g = 2u, .b = colorSetVal, .band = e160Hz};
static const frqBand_t bl2 = {.gCv = getLChanVal, .pSt = 52u, .pL = 26u, .r = colorSetVal, .g = colorSetVal, .b = 1u, .band = e400Hz};
static const frqBand_t br2 = {.gCv = getRChanVal, .pSt = (618u -90), .pL = 30u, .r = colorSetVal, .g = colorSetVal, .b = 1u, .band = e400Hz};
static const frqBand_t bl3 = {.gCv = getLChanVal, .pSt = 78u, .pL = 26u, .r = 1u, .g = colorSetVal, .b = 1u, .band = e1kHz};
static const frqBand_t br3 = {.gCv = getRChanVal, .pSt = (618u -120), .pL = 30u, .r = 1u, .g = colorSetVal, .b = 1u, .band = e1kHz};
static const frqBand_t bl4 = {.gCv = getLChanVal, .pSt = 104u, .pL = 26u, .r = colorSetVal/2, .g = colorSetVal/2, .b = colorSetVal, .band = e2_5kHz};
static const frqBand_t br4 = {.gCv = getRChanVal, .pSt = (618u -150), .pL = 30u, .r = colorSetVal/2, .g = colorSetVal/2, .b = colorSetVal, .band = e2_5kHz};
static const frqBand_t bl5 = {.gCv = getLChanVal, .pSt = 130u, .pL = 26u, .r = colorSetVal, .g = 1u, .b = 1u, .band = e6_25kHz};
static const frqBand_t br5 = {.gCv = getRChanVal, .pSt = (618u -180), .pL = 30u, .r = colorSetVal, .g = 1u, .b = 1u, .band = e6_25kHz};
static const frqBand_t bl6 = {.gCv = getLChanVal, .pSt = 156u, .pL = 26u, .r = colorSetVal/2, .g = colorSetVal, .b = colorSetVal/2, .band = e16kHz};
static const frqBand_t br6 = {.gCv = getRChanVal, .pSt = (618u -210), .pL = 30u, .r = colorSetVal/2, .g = colorSetVal, .b = colorSetVal/2, .band = e16kHz};
const frqBand_t *l[] = {&bl0, &br0, &bl1, &br1, &bl2, &br2, &bl3, &br3, &bl4, &br4, &bl5, &br5, &bl6, &br6, NULL};

void anim_frqDrv(LedChainDesc_t *const lcd, const frqBand_t *fB)
{
	static const uint32_t max = MAX_OUT;
	static const uint32_t round = max / 2uL;
	uint32_t val = fB->gCv(fB->band);

	uint32_t scaled = (((fB->pL / 2) * val) + round) / max;
	uint32_t remainder = ((fB->pL / 2) * val) % max;
	uint32_t remainderFull = val % max;
//	uint8_t remR = (uint8_t) (((uint32_t) (remainder * fB->r) / max) + 1u);
//	uint8_t remG = (uint8_t) (((uint32_t) (remainder * fB->g) / max) + 1u);
//	uint8_t remB = (uint8_t) (((uint32_t) (remainder * fB->b) / max) + 1u);
	uint8_t remRF = (uint8_t) (((uint32_t) (remainderFull * fB->r) / max) + 1u);
	uint8_t remGF = (uint8_t) (((uint32_t) (remainderFull * fB->g) / max) + 1u);
	uint8_t remBF = (uint8_t) (((uint32_t) (remainderFull * fB->b) / max) + 1u);

	for (uint32_t i = 0; i < fB->pL; ++i) {
		led_setLedToColor(lcd, fB->pSt + i, remRF, remGF, remBF);
	}

}
