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

#include "animations.h"

static const frqBand_t bl = {.gCv = getLChanVal, .pSt = 0u, .pL = 15u, .r = 1, .g = 1, .b = 255u, .band = e63Hz};
static const frqBand_t br = {.gCv = getRChanVal, .pSt = 0u, .pL = 15u, .band = e63Hz};
static const frqBand_t *l[] = {};
//typedef

void anim_frqDrv(LedChainDesc_t *const lcd, const frqBand_t *fB)
{
	static const uint32_t max = MAX_OUT;
	static const uint32_t round = max / 2uL;
	uint32_t val = fB->gCv(fB->band);

	uint32_t scaled = (((bl.pL / 2) * val) + round) / max;
	uint32_t remainder = ((bl.pL / 2) * val) % max;
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
