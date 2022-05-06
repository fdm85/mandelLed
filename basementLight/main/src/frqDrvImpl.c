/**
 * @file      frqDrvImpl.c
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
