/**
 * @file      matrix.c
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
 * @brief Matrix implementation
 * @ingroup Matrix
 * @{
 */

#include "matrix.h"
#include "msgeq7.h"
#include "assrt.h"

typedef struct bar_tag {
   uint8_t dots[barHeigth];
} bar_t;

typedef struct channel_tag {
   bar_t bar[barWidth];
} channel_t;

static channel_t lLeft[channelWidth];
static channel_t lRight[channelWidth];
static bool inverted = true;

static channel_t lAuxLeft[auxChanWidth];
static channel_t lAuxRight[auxChanWidth];

static void mtrx_InitAuxL(void) {
   uint8_t L14[barHeigth] = { 241u, 206u, 177u, 142u, 113u, 78u, 49u, 14u };
   uint8_t L15[barHeigth] = { 240u, 207u, 176u, 143u, 112u, 79u, 48u, 15u };

   for (uint8_t i = 0; i < barHeigth; ++i) {
      lAuxLeft[0].bar[0].dots[i] = L14[i];
      lAuxLeft[0].bar[1].dots[i] = L15[i];
   }
}

static void mtrx_InitAuxR(void) {
   uint8_t R14[barHeigth] = { 238u, 209u, 174u, 145u, 110u, 81u, 46u, 17u };
   uint8_t R15[barHeigth] = { 239u, 208u, 175u, 144u, 111u, 80u, 47u, 16u };

   for (uint8_t i = 0; i < barHeigth; ++i) {
      lAuxRight[0].bar[0].dots[i] = R14[i];
      lAuxRight[0].bar[1].dots[i] = R15[i];
   }
}

static void mtrx_InitL(void) {
   uint8_t L0[barHeigth] = { 255u, 192u, 191u, 128u, 127u, 64u, 63u, 0u };
   uint8_t L1[barHeigth] = { 254u, 193u, 190u, 129u, 126u, 65u, 62u, 1u };

   uint8_t L2[barHeigth] = { 253u, 194u, 189u, 130u, 125u, 66u, 61u, 2u };
   uint8_t L3[barHeigth] = { 252u, 195u, 188u, 131u, 124u, 67u, 60u, 3u };

   uint8_t L4[barHeigth] = { 251u, 196u, 187u, 132u, 123u, 68u, 59u, 4u };
   uint8_t L5[barHeigth] = { 250u, 197u, 186u, 133u, 122u, 69u, 58u, 5u };

   uint8_t L6[barHeigth] = { 249u, 198u, 185u, 134u, 121u, 70u, 57u, 6u };
   uint8_t L7[barHeigth] = { 248u, 199u, 184u, 135u, 120u, 71u, 56u, 7u };

   uint8_t L8[barHeigth] = { 247u, 200u, 183u, 136u, 119u, 72u, 55u, 8u };
   uint8_t L9[barHeigth] = { 246u, 201u, 182u, 137u, 118u, 73u, 54u, 9u };

   uint8_t L10[barHeigth] = { 245u, 202u, 181u, 138u, 117u, 74u, 53u, 10u };
   uint8_t L11[barHeigth] = { 244u, 203u, 180u, 139u, 116u, 75u, 52u, 11u };

   uint8_t L12[barHeigth] = { 243u, 204u, 179u, 140u, 115u, 76u, 51u, 12u };
   uint8_t L13[barHeigth] = { 242u, 205u, 178u, 141u, 114u, 77u, 50u, 13u };

   for (uint8_t i = 0; i < barHeigth; ++i) {
      lLeft[0].bar[0].dots[i] = L0[i];
      lLeft[0].bar[1].dots[i] = L1[i];

      lLeft[1].bar[0].dots[i] = L2[i];
      lLeft[1].bar[1].dots[i] = L3[i];

      lLeft[2].bar[0].dots[i] = L4[i];
      lLeft[2].bar[1].dots[i] = L5[i];

      lLeft[3].bar[0].dots[i] = L6[i];
      lLeft[3].bar[1].dots[i] = L7[i];

      lLeft[4].bar[0].dots[i] = L8[i];
      lLeft[4].bar[1].dots[i] = L9[i];

      lLeft[5].bar[0].dots[i] = L10[i];
      lLeft[5].bar[1].dots[i] = L11[i];

      lLeft[6].bar[0].dots[i] = L12[i];
      lLeft[6].bar[1].dots[i] = L13[i];
   }
}

static void mtrx_InitR(void) {
   uint8_t R0[barHeigth] = { 224u, 223u, 160u, 159u, 96u, 95u, 32u, 31u };
   uint8_t R1[barHeigth] = { 225u, 222u, 161u, 158u, 97u, 94u, 33u, 30u };

   uint8_t R2[barHeigth] = { 226u, 221u, 162u, 157u, 98u, 93u, 34u, 29u };
   uint8_t R3[barHeigth] = { 227u, 220u, 163u, 156u, 99u, 92u, 35u, 28u };

   uint8_t R4[barHeigth] = { 228u, 219u, 164u, 155u, 100u, 91u, 36u, 27u };
   uint8_t R5[barHeigth] = { 229u, 218u, 165u, 154u, 101u, 90u, 37u, 26u };

   uint8_t R6[barHeigth] = { 230u, 217u, 166u, 153u, 102u, 89u, 38u, 25u };
   uint8_t R7[barHeigth] = { 231u, 216u, 167u, 152u, 103u, 88u, 39u, 24u };

   uint8_t R8[barHeigth] = { 232u, 215u, 168u, 151u, 104u, 87u, 40u, 23u };
   uint8_t R9[barHeigth] = { 233u, 214u, 169u, 150u, 105u, 86u, 41u, 22u };

   uint8_t R10[barHeigth] = { 234u, 213u, 170u, 149u, 106u, 85u, 42u, 21u };
   uint8_t R11[barHeigth] = { 235u, 212u, 171u, 148u, 107u, 84u, 43u, 20u };

   uint8_t R12[barHeigth] = { 236u, 211u, 172u, 147u, 108u, 83u, 44u, 19u };
   uint8_t R13[barHeigth] = { 237u, 210u, 173u, 146u, 109u, 82u, 45u, 18u };

   for (uint8_t i = 0; i < barHeigth; ++i) {
      lRight[0].bar[0].dots[i] = R0[i];
      lRight[0].bar[1].dots[i] = R1[i];

      lRight[1].bar[0].dots[i] = R2[i];
      lRight[1].bar[1].dots[i] = R3[i];

      lRight[2].bar[0].dots[i] = R4[i];
      lRight[2].bar[1].dots[i] = R5[i];

      lRight[3].bar[0].dots[i] = R6[i];
      lRight[3].bar[1].dots[i] = R7[i];

      lRight[4].bar[0].dots[i] = R8[i];
      lRight[4].bar[1].dots[i] = R9[i];

      lRight[5].bar[0].dots[i] = R10[i];
      lRight[5].bar[1].dots[i] = R11[i];

      lRight[6].bar[0].dots[i] = R12[i];
      lRight[6].bar[1].dots[i] = R13[i];
   }
}

void mtrx_Init(void) {
   mtrx_InitL();
   mtrx_InitR();
   mtrx_InitAuxL();
   mtrx_InitAuxR();
}

static void mtrx_setLedsScaled(LedChainDesc_t *const lcd, channel_t *chan, uint32_t val, uint8_t r, uint8_t g, uint8_t b) {
   static const uint32_t max = 3100uL;
   static const uint32_t round = max / 2uL;

   uint32_t scaled = ((barHeigth * val) + round) / max;

   if (scaled > barHeigth)
      scaled = barHeigth;

   assrt(scaled <= barHeigth);

   for (uint8_t i = 0; i < scaled; ++i) {
      led_setLedToColor(lcd, chan->bar[0].dots[inverted ? (barHeigth - (i + 1)) : i], r, g, b);
      led_setLedToColor(lcd, chan->bar[1].dots[inverted ? (barHeigth - (i + 1)) : i], r, g, b);
   }
}
static void mtrx_setLedsScaled2(LedChainDesc_t *const lcd, channel_t *chan, uint32_t val1, uint32_t val2, uint8_t r, uint8_t g, uint8_t b) {
   static const uint32_t max = 3100uL;
   static const uint32_t round = max / 2uL;

   uint32_t scaled1 = ((barHeigth * val1) + round) / max;
   uint32_t scaled2 = ((barHeigth * val2) + round) / max;

   if (scaled1 > barHeigth)
      scaled1 = barHeigth;

   assrt(scaled1 <= barHeigth);
   if (scaled2 > barHeigth)
      scaled2 = barHeigth;

   assrt(scaled2 <= barHeigth);

   for (uint8_t i = 0; i < scaled1; ++i) {
      led_setLedToColor(lcd, chan->bar[0].dots[inverted ? (barHeigth - (i + 1)) : i], r, g, b);
   }
   for (uint8_t i = 0; i < scaled2; ++i) {
      led_setLedToColor(lcd, chan->bar[1].dots[inverted ? (barHeigth - (i + 1)) : i], r, g, b);
   }
}

static void mtrx_setAuxLedsScaled(LedChainDesc_t *const lcd, channel_t *chan, uint32_t val, uint8_t r, uint8_t g, uint8_t b) {
   static const uint32_t max = (8 * MAX_OUT);
   static const uint32_t round = max / 2uL;

   uint32_t scaled = (((barHeigth / 2) * val) + round) / max;
   uint32_t remainder = ((barHeigth / 2) * val) % max;
   uint32_t remainderFull = val % max;
   uint8_t remR = (uint8_t) (((uint32_t) (remainder * r) / max) + 1u);
   uint8_t remG = (uint8_t) (((uint32_t) (remainder * g) / max) + 1u);
   uint8_t remB = (uint8_t) (((uint32_t) (remainder * b) / max) + 1u);
   uint8_t remRF = (uint8_t) (((uint32_t) (remainderFull * r) / max) + 1u);
   uint8_t remGF = (uint8_t) (((uint32_t) (remainderFull * g) / max) + 1u);
   uint8_t remBF = (uint8_t) (((uint32_t) (remainderFull * b) / max) + 1u);

   if (remB < 2) {
      remB = 2;
      if (remR < 2)
         remR = 2;
      if (remG < 2)
         remG = 2;
   }
   if (remBF < 2u) {
      remBF = 2u;
      if (remRF < 2u)
         remRF = 2u;
      if (remGF < 2u)
         remGF = 2u;
   }
   if (scaled > (barHeigth / 2))
      scaled = (barHeigth / 2);

   assrt(scaled <= (barHeigth/2));
   uint8_t i;
   for (i = (barHeigth / 2u); i < ((barHeigth / 2) + scaled); ++i) {
      led_setLedToColor(lcd, chan->bar[1].dots[i], r, g, b);
   }
   uint8_t j;
   for (j = 0; j < scaled; ++j) {
      led_setLedToColor(lcd, chan->bar[1].dots[((barHeigth / 2u) - 1) - j], r, g, b);
   }
   if (scaled < (barHeigth / 2)) {
      led_setLedToColor(lcd, chan->bar[0].dots[3], remRF, remGF, remBF);
      led_setLedToColor(lcd, chan->bar[0].dots[4], remRF, remGF, remBF);
      led_setLedToColor(lcd, chan->bar[1].dots[i], remR, remG, remB);
      led_setLedToColor(lcd, chan->bar[1].dots[((barHeigth / 2u) - 1) - j], remR, remG, remB);
   }
}

void mtrx_anim(mAnim_t *ctx) {
   static uint8_t color = 20u;
   static uint32_t auxLeft = 0uL;
   static uint32_t auxRight = 0uL;

   auxLeft = getLChanVal(e63Hz) + getLChanVal(e160Hz) + getLChanVal(e400Hz) + getLChanVal(e1kHz) + getLChanVal(e2_5kHz) + getLChanVal(e6_25kHz) + getLChanVal(e16kHz);
   auxRight = getRChanVal(e63Hz) + getRChanVal(e160Hz) + getRChanVal(e400Hz) + getRChanVal(e1kHz) + getRChanVal(e2_5kHz) + getRChanVal(e6_25kHz) + getRChanVal(e16kHz);

   led_setAllLedsToColor(ctx->lcd_ctx, 2u, 2u, 2u);
   mtrx_setAuxLedsScaled(ctx->lcd_ctx, &lAuxLeft[0], auxLeft, 3u, 3u, color);
   mtrx_setAuxLedsScaled(ctx->lcd_ctx, &lAuxRight[0], auxRight, 3u, 3u, color);
   // comparison mode
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lLeft[0], getLChanVal(e63Hz), getLChanVal2(e63Hz), 2u, color, color);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lLeft[1], getLChanVal(e160Hz), getLChanVal2(e160Hz), color, 2u, color);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lLeft[2], getLChanVal(e400Hz), getLChanVal2(e400Hz), color, color, 2u);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lLeft[3], getLChanVal(e1kHz), getLChanVal2(e1kHz), 2u, color, 2u);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lLeft[4], getLChanVal(e2_5kHz), getLChanVal2(e2_5kHz), color / 2, color / 2, color);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lLeft[5], getLChanVal(e6_25kHz), getLChanVal2(e6_25kHz), color, 2u, 2u);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lLeft[6], getLChanVal(e16kHz), getLChanVal2(e16kHz), color / 2, color, color / 2);
//
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lRight[0], getRChanVal(e63Hz), getRChanVal2(e63Hz), 2u, color, color);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lRight[1], getRChanVal(e160Hz), getRChanVal2(e160Hz), color, 2u, color);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lRight[2], getRChanVal(e400Hz), getRChanVal2(e400Hz), color, color, 2u);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lRight[3], getRChanVal(e1kHz), getRChanVal2(e1kHz), 2u, color, 2u);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lRight[4], getRChanVal(e2_5kHz), getRChanVal2(e2_5kHz), color / 2, color / 2, color);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lRight[5], getRChanVal(e6_25kHz), getRChanVal2(e6_25kHz), color, 2u, 2u);
//   mtrx_setLedsScaled2(ctx->lcd_ctx, &lRight[6], getRChanVal(e16kHz), getRChanVal2(e16kHz), color / 2, color, color / 2);
   // comparions mode
   mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[0], getLChanVal2(e63Hz), 2u, color, color);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[1], getLChanVal2(e160Hz), color, 2u, color);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[2], getLChanVal2(e400Hz), color, color, 2u);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[3], getLChanVal2(e1kHz), 2u, color, 2u);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[4], getLChanVal2(e2_5kHz), color / 2, color / 2, color);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[5], getLChanVal2(e6_25kHz), color, 2u, 2u);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[6], getLChanVal2(e16kHz), color / 2, color, color / 2);

   mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[0], getRChanVal2(e63Hz), 2u, color, color);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[1], getRChanVal2(e160Hz), color, 2u, color);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[2], getRChanVal2(e400Hz), color, color, 2u);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[3], getRChanVal2(e1kHz), 2u, color, 2u);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[4], getRChanVal2(e2_5kHz), color / 2, color / 2, color);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[5], getRChanVal2(e6_25kHz), color, 2u, 2u);
   mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[6], getRChanVal2(e16kHz), color / 2, color, color / 2);
}
/** @}*/
