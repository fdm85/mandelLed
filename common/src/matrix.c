/******************************************************************************/
/*!
 * \file
 *
 * \brief info: add file brief
 *
 * todo: add file description
 ******************************************************************************/

#include "matrix.h"
#include "msgeq7.h"
#include "assrt.h"


typedef struct bar_tag {
	uint8_t dots[barHeigth];
}bar_t;

typedef struct channel_tag {
	bar_t bar[barWidth];
}channel_t;

static channel_t lLeft[channelWidth];
static channel_t lRight[channelWidth];
static bool inverted = true;

static channel_t lAuxLeft[auxChanWidth];
static channel_t lAuxRight[auxChanWidth];

static void mtrx_InitAuxL(void)
{
	uint8_t L14[barHeigth] =
	{ 241u, 206u, 177u, 142u, 113u, 78u, 49u, 14u };
	uint8_t L15[barHeigth] =
	{ 240u, 207u, 176u, 143u, 112u, 79u, 48u, 15u };
}

static void mtrx_InitAuxR(void)
{
	uint8_t R14[barHeigth] =
	{ 238u, 209u, 174u, 145u, 110u, 81u, 46u, 17u };
	uint8_t R15[barHeigth] =
	{ 239u, 208u, 175u, 144u, 111u, 80u, 47u, 16u };
}

static void mtrx_InitL(void)
{
	uint8_t L0[barHeigth] =
	{ 255u, 192u, 191u, 128u, 127u, 64u, 63u, 0u };
	uint8_t L1[barHeigth] =
	{ 254u, 193u, 190u, 129u, 126u, 65u, 62u, 1u };

	uint8_t L2[barHeigth] =
	{ 253u, 194u, 189u, 130u, 125u, 66u, 61u, 2u };
	uint8_t L3[barHeigth] =
	{ 252u, 195u, 188u, 131u, 124u, 67u, 60u, 3u };

	uint8_t L4[barHeigth] =
	{ 251u, 196u, 187u, 132u, 123u, 68u, 59u, 4u };
	uint8_t L5[barHeigth] =
	{ 250u, 197u, 186u, 133u, 122u, 69u, 58u, 5u };

	uint8_t L6[barHeigth] =
	{ 249u, 198u, 185u, 134u, 121u, 70u, 57u, 6u };
	uint8_t L7[barHeigth] =
	{ 248u, 199u, 184u, 135u, 120u, 71u, 56u, 7u };

	uint8_t L8[barHeigth] =
	{ 247u, 200u, 183u, 136u, 119u, 72u, 55u, 8u };
	uint8_t L9[barHeigth] =
	{ 246u, 201u, 182u, 137u, 118u, 73u, 54u, 9u };

	uint8_t L10[barHeigth] =
	{ 245u, 202u, 181u, 138u, 117u, 74u, 53u, 10u };
	uint8_t L11[barHeigth] =
	{ 244u, 203u, 180u, 139u, 116u, 75u, 52u, 11u };

	uint8_t L12[barHeigth] =
	{ 243u, 204u, 179u, 140u, 115u, 76u, 51u, 12u };
	uint8_t L13[barHeigth] =
	{ 242u, 205u, 178u, 141u, 114u, 77u, 50u, 13u };

	for (uint8_t i = 0; i < barHeigth; ++i)
	{
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

static void mtrx_InitR(void)
{
	uint8_t R0[barHeigth] =
	{ 224u, 223u, 160u, 159u, 96u, 95u, 32u, 31u };
	uint8_t R1[barHeigth] =
	{ 225u, 222u, 161u, 158u, 97u, 94u, 33u, 30u };

	uint8_t R2[barHeigth] =
	{ 226u, 221u, 162u, 157u, 98u, 93u, 34u, 29u };
	uint8_t R3[barHeigth] =
	{ 227u, 220u, 163u, 156u, 99u, 92u, 35u, 28u };

	uint8_t R4[barHeigth] =
	{ 228u, 219u, 164u, 155u, 100u, 91u, 36u, 27u };
	uint8_t R5[barHeigth] =
	{ 229u, 218u, 165u, 154u, 101u, 90u, 37u, 26u };

	uint8_t R6[barHeigth] =
	{ 230u, 217u, 166u, 153u, 102u, 89u, 38u, 25u };
	uint8_t R7[barHeigth] =
	{ 231u, 216u, 167u, 152u, 103u, 88u, 39u, 24u };

	uint8_t R8[barHeigth] =
	{ 232u, 215u, 168u, 151u, 104u, 87u, 40u, 23u };
	uint8_t R9[barHeigth] =
	{ 233u, 214u, 169u, 150u, 105u, 86u, 41u, 22u };

	uint8_t R10[barHeigth] =
	{ 234u, 213u, 170u, 149u, 106u, 85u, 42u, 21u };
	uint8_t R11[barHeigth] =
	{ 235u, 212u, 171u, 148u, 107u, 84u, 43u, 20u };

	uint8_t R12[barHeigth] =
	{ 236u, 211u, 172u, 147u, 108u, 83u, 44u, 19u };
	uint8_t R13[barHeigth] =
	{ 237u, 210u, 173u, 146u, 109u, 82u, 45u, 18u };

	for (uint8_t i = 0; i < barHeigth; ++i)
	{
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

void mtrx_Init(void)
{
	mtrx_InitL();
	mtrx_InitR();
	mtrx_InitAuxL();
	mtrx_InitAuxR();
}

static void mtrx_setLedsScaled(LedChainDesc_t *const lcd, channel_t *chan, uint32_t val, uint8_t r, uint8_t g,
		uint8_t b)
{
	static const uint32_t max = 3100uL;
	static const uint32_t round = max / 2uL;

	uint32_t scaled = ((barHeigth * val) + round) / max;

	if (scaled > barHeigth)
		scaled = barHeigth;

	assrt(scaled <= barHeigth);

	for (uint8_t i = 0; i < scaled; ++i)
	{
		led_setLedToColor(lcd, chan->bar[0].dots[inverted ? (barHeigth - (i + 1)) : i], r, g, b);
		led_setLedToColor(lcd, chan->bar[1].dots[inverted ? (barHeigth - (i + 1)) : i], r, g, b);
	}
}

void mtrx_anim(mAnim_t* ctx)
{
	static uint8_t color = 80u;
	led_setAllLedsToColor(ctx->lcd_ctx, 15, 15, 15);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[0], getLChanVal(e63Hz), 0u, color, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[1], getLChanVal(e160Hz), color, 0u, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[2], getLChanVal(e400Hz), color, color, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[3], getLChanVal(e1kHz), 0u, color, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[4], getLChanVal(e2_5kHz), 0u, 0u, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[5], getLChanVal(e6_25kHz), color, 0u, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lLeft[6], getLChanVal(e16kHz), color, color, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[0], getRChanVal(e63Hz), 0u, color, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[1], getRChanVal(e160Hz), color, 0u, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[2], getRChanVal(e400Hz), color, color, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[3], getRChanVal(e1kHz), 0u, color, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[4], getRChanVal(e2_5kHz), 0u, 0u, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[5], getRChanVal(e6_25kHz), color, 0u, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lRight[6], getRChanVal(e16kHz), color, color, color);
}
