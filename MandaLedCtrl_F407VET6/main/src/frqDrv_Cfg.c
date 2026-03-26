/**
 * @file      frqDrv_Cfg.c
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
 * @brief Defines target depending frq-driven animations
 * @defgroup FrqDrv_Cfg Configuration of frequency driven animation style
 * @ingroup FrqDrv
 * @ingroup BasementLight
 * @{
 */


/*! @brief brightness value */
#define colorSetVal 100u
#include "animations.h"
#include "leds.h"
fm_frqBand(rl0, getLChanVal2,   0u,  59u, 0u, 0u, colorSetVal, e63Hz, MAX_OUT);
fm_frqBand(rl1, getLChanVal2,  60u, 120u, colorSetVal, 0u, colorSetVal, e160Hz, MAX_OUT);
fm_frqBand(bl2, getLChanVal2, 121u, 140u, colorSetVal, 0u, 0u, e400Hz, MAX_OUT);
fm_frqBand(bl3, getLChanVal2, 141u, 160u, colorSetVal, colorSetVal, 0u, e1kHz, MAX_OUT);
fm_frqBand(bl4, getLChanVal2, 161u, 180u, 0u, 0u, colorSetVal, e2_5kHz, MAX_OUT);
fm_frqBand(bl5, getLChanVal2, 181u, MAIN_L_LDCNT, 0u, colorSetVal, 0u, e6_25kHz, MAX_OUT);
//fm_frqBand(bl6, getLChanVal2, 201u, MAIN_L_LDCNT, colorSetVal, colorSetVal, 0u, e16kHz, MAX_OUT);
// 250

fm_frqBand(rr0, getRChanVal2,   0u,  59u,  0u, 0u, colorSetVal, e63Hz, MAX_OUT);
fm_frqBand(rr1, getRChanVal2,  60u, 120u, colorSetVal, 0u, colorSetVal, e160Hz, MAX_OUT);
fm_frqBand(br2, getRChanVal2, 121u, 140u, colorSetVal, 0u, 0u, e400Hz, MAX_OUT);
fm_frqBand(br3, getRChanVal2, 141u, 180u, colorSetVal, colorSetVal, 0u, e1kHz, MAX_OUT);
fm_frqBand(br4, getRChanVal2, 181u, 220u, 0u, 0u, colorSetVal, e2_5kHz, MAX_OUT);
fm_frqBand(br5, getRChanVal2, 221u, MAIN_R_LDCNT, 0u, colorSetVal, 0u, e6_25kHz, MAX_OUT);
//fm_frqBand(br6, getRChanVal2, 441u, MAIN_R_LDCNT, colorSetVal, colorSetVal, 0u, e16kHz, MAX_OUT);


/*! @brief Left side config frequency-band to position config */
const frqBand_t *frqL[] = { &bl2, &bl3, &bl4, &bl5, NULL };
/*! @brief Right side frequency-band to position config */
const frqBand_t *frqR[] = { &br2, &br3, &br4, &br5, NULL };
/*! @brief Middle side config frequency-band to position config */
const frqBand_t *frqML[] = { &rl0, &rl1, NULL };
const frqBand_t *frqMR[] = { &rr0, &rr1, NULL };
/*! @brief Left side animation function selection and default coloring */
const frqString_t frqL_Str = {.f = anim_frqDrv, .start = 200u, .end = MAIN_L_LDCNT, .backGround = {.r = 15u, .g = 15u, .b = 15u }, .frqB = frqL};
const frqString_t frqR_Str = {.f = anim_frqDrv, .start = 200u, .end = MAIN_R_LDCNT, .backGround = {.r = 15u, .g = 15u, .b = 15u }, .frqB = frqR};
/*! @brief Middle side animation function selection and default coloring */
const frqString_t frqML_Str = {.f = anim_frqFrvRem, .start = 0u, .end = 200u, .backGround = {.r = 60u, .g = 3u, .b = 8u },.frqB = frqML};
const frqString_t frqMR_Str = {.f = anim_frqFrvRem, .start = 0u, .end = 200u, .backGround = {.r = 60u, .g = 3u, .b = 8u },.frqB = frqMR};
/*! @brief Export of config via list */
const frqString_t *fL[] = {&frqL_Str, &frqML_Str, NULL};
const frqString_t *fR[] = {&frqR_Str, &frqMR_Str, NULL};

void anim_frqDrvL(mAnim_t *ctx) {
  for (uint8_t i = 0; fL[i] != NULL; ++i) {
    led_setFromToLedsToColor(ctx->lcd_ctx, fL[i]->backGround.r, fL[i]->backGround.g, fL[i]->backGround.b, fL[i]->start, fL[i]->end);
    for (uint8_t j = 0; fL[i]->frqB[j] != NULL; ++j) {
      fL[i]->f(ctx->lcd_ctx, fL[i]->frqB[j]);
    }
  }
}

void anim_frqDrvR(mAnim_t *ctx) {
  for (uint8_t i = 0; fR[i] != NULL; ++i) {
    led_setFromToLedsToColor(ctx->lcd_ctx, fR[i]->backGround.r, fR[i]->backGround.g, fR[i]->backGround.b, fR[i]->start, fR[i]->end);
    for (uint8_t j = 0; fR[i]->frqB[j] != NULL; ++j) {
      fR[i]->f(ctx->lcd_ctx, fR[i]->frqB[j]);
    }
  }
}

/** @}*/
