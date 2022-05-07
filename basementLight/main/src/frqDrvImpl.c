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

fm_frqBand(bl6, getLChanVal2, 0u, 27u, colorSetVal, colorSetVal, 0u, e16kHz, MAX_OUT);
fm_frqBand(bl5, getLChanVal2, 27u, 27u, 0u, colorSetVal, 0u, e6_25kHz, MAX_OUT);
fm_frqBand(bl4, getLChanVal2, 54u, 27u, 0u, 0u, colorSetVal, e2_5kHz, MAX_OUT);
fm_frqBand(bl3, getLChanVal2, 81u, 27u, colorSetVal, colorSetVal, 0u, e1kHz, MAX_OUT);
fm_frqBand(bl2, getLChanVal2, 108u, 31u, colorSetVal, 0u, 0u, e400Hz, MAX_OUT);
fm_frqBand(rl1, getLChanVal2, 139u, 45u, colorSetVal, 0u, colorSetVal, e160Hz, MAX_OUT);
fm_frqBand(rl0, getLChanVal2, 184u, 66u, 0u, 0u, colorSetVal, e63Hz, MAX_OUT);
// 250

fm_frqBand(br6, getRChanVal2, 570u, 49u, colorSetVal, colorSetVal, 0u, e16kHz, MAX_OUT);
fm_frqBand(br5, getRChanVal2, 521u, 49u, 0u, colorSetVal, 0u, e6_25kHz, MAX_OUT);
fm_frqBand(br4, getRChanVal2, 472u, 49u, 0u, 0u, colorSetVal, e2_5kHz, MAX_OUT);
fm_frqBand(br3, getRChanVal2, 423u, 49u, colorSetVal, colorSetVal, 0u, e1kHz, MAX_OUT);
fm_frqBand(br2, getRChanVal2, 374u, 49u, colorSetVal, 0u, 0u, e400Hz, MAX_OUT);
fm_frqBand(rr1, getRChanVal2, 319u, 55u, colorSetVal, 0u, colorSetVal, e160Hz, MAX_OUT);
fm_frqBand(rr0, getRChanVal2, 253u, 66u, 0u, 0u, colorSetVal, e63Hz, MAX_OUT);



const frqBand_t *frqR[] = { &br2, &br3, &br4, &br5, &br6, NULL };
const frqBand_t *frqL[] = { &bl2, &bl3, &bl4, &bl5, &bl6, NULL };
const frqBand_t *frqM[] = { &rl0, &rr0, &rl1, &rr1, NULL };
const frqString_t frqL_Str = {.f = anim_frqDrv, .start = 0u, .end = 138u, .backGround = {.r = 15u, .g = 15u, .b = 15u }, .frqB = frqL};
const frqString_t frqR_Str = {.f = anim_frqDrv, .start = 374u, .end = 619u, .backGround = {.r = 15u, .g = 15u, .b = 15u }, .frqB = frqR};
const frqString_t frqM_Str = {.f = anim_frqFrvRem, .start = 139u, .end = 373u, .backGround = {.r = 60u, .g = 3u, .b = 8u },.frqB = frqM};

const frqString_t *frqS[] = { &frqL_Str, &frqR_Str, &frqM_Str, NULL};
