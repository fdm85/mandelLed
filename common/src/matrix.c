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

static channel_t left[channelWidth];
static channel_t lright[channelWidth];

static void mtrx_InitL(void)
{
	uint8_t L0[8] =
	{ 254u, 191u, 190u, 127u, 126u, 64u, 63u, 0u };
	uint8_t L1[8] =
	{ 253u, 192u, 189u, 128u, 125u, 65u, 62u, 1u };

	uint8_t L2[8] =
	{ 252u, 193u, 188u, 129u, 124u, 66u, 61u, 2u };
	uint8_t L3[8] =
	{ 251u, 194u, 187u, 130u, 123u, 67u, 60u, 3u };

	uint8_t L4[8] =
	{ 250u, 195u, 186u, 131u, 122u, 68u, 59u, 4u };
	uint8_t L5[8] =
	{ 249u, 196u, 185u, 132u, 121u, 69u, 58u, 5u };

	uint8_t L6[8] =
	{ 248u, 197u, 184u, 133u, 120u, 70u, 57u, 6u };
	uint8_t L7[8] =
	{ 247u, 198u, 183u, 134u, 119u, 71u, 56u, 7u };

	uint8_t L8[8] =
	{ 246u, 199u, 182u, 135u, 118u, 72u, 55u, 8u };
	uint8_t L9[8] =
	{ 245u, 200u, 181u, 136u, 117u, 73u, 54u, 9u };

	uint8_t L10[8] =
	{ 244u, 201u, 180u, 137u, 116u, 74u, 53u, 10u };
	uint8_t L11[8] =
	{ 243u, 202u, 179u, 138u, 115u, 75u, 52u, 11u };

	uint8_t L12[8] =
	{ 242u, 203u, 178u, 139u, 114u, 76u, 51u, 12u };
	uint8_t L13[8] =
	{ 241u, 204u, 177u, 140u, 113u, 77u, 50u, 13u };

	for (uint8_t i = 0; i < barHeigth; ++i)
	{
		left[0].bar[0].dots[i] = L0[i];
		left[0].bar[1].dots[i] = L1[i];

		left[1].bar[0].dots[i] = L2[i];
		left[1].bar[1].dots[i] = L3[i];

		left[2].bar[0].dots[i] = L4[i];
		left[2].bar[1].dots[i] = L5[i];

		left[3].bar[0].dots[i] = L6[i];
		left[3].bar[1].dots[i] = L7[i];

		left[4].bar[0].dots[i] = L8[i];
		left[4].bar[1].dots[i] = L9[i];

		left[5].bar[0].dots[i] = L10[i];
		left[5].bar[1].dots[i] = L11[i];

		left[6].bar[0].dots[i] = L12[i];
		left[6].bar[1].dots[i] = L13[i];
	}
}

static void mtrx_InitR(void)
{
	uint8_t R0[8] =
	{ 223u, 222u, 159u, 158u, 96u, 95u, 32u, 31u };
	uint8_t R1[8] =
	{ 224u, 221u, 160u, 157u, 97u, 94u, 33u, 30u };

	uint8_t R2[8] =
	{ 225u, 220u, 161u, 156u, 98u, 93u, 34u, 29u };
	uint8_t R3[8] =
	{ 226u, 219u, 162u, 155u, 99u, 92u, 35u, 28u };

	uint8_t R4[8] =
	{ 227u, 218u, 163u, 154u, 100u, 91u, 36u, 27u };
	uint8_t R5[8] =
	{ 228u, 217u, 164u, 153u, 101u, 90u, 37u, 26u };

	uint8_t R6[8] =
	{ 229u, 216u, 165u, 152u, 102u, 89u, 38u, 25u };
	uint8_t R7[8] =
	{ 230u, 215u, 166u, 151u, 103u, 88u, 39u, 24u };

	uint8_t R8[8] =
	{ 231u, 214u, 167u, 150u, 104u, 87u, 40u, 23u };
	uint8_t R9[8] =
	{ 232u, 213u, 168u, 149u, 105u, 86u, 41u, 22u };

	uint8_t R10[8] =
	{ 233u, 212u, 169u, 148u, 106u, 85u, 42u, 21u };
	uint8_t R11[8] =
	{ 234u, 211u, 170u, 147u, 107u, 84u, 43u, 20u };

	uint8_t R12[8] =
	{ 235u, 210u, 171u, 146u, 108u, 83u, 44u, 19u };
	uint8_t R13[8] =
	{ 236u, 209u, 172u, 145u, 109u, 82u, 45u, 18u };

	for (uint8_t i = 0; i < barHeigth; ++i)
	{
		lright[0].bar[0].dots[i] = R0[i];
		lright[0].bar[1].dots[i] = R1[i];

		lright[1].bar[0].dots[i] = R2[i];
		lright[1].bar[1].dots[i] = R3[i];

		lright[2].bar[0].dots[i] = R4[i];
		lright[2].bar[1].dots[i] = R5[i];

		lright[3].bar[0].dots[i] = R6[i];
		lright[3].bar[1].dots[i] = R7[i];

		lright[4].bar[0].dots[i] = R8[i];
		lright[4].bar[1].dots[i] = R9[i];

		lright[5].bar[0].dots[i] = R10[i];
		lright[5].bar[1].dots[i] = R11[i];

		lright[6].bar[0].dots[i] = R12[i];
		lright[6].bar[1].dots[i] = R13[i];
	}
}

void mtrx_Init(void)
{
	mtrx_InitL();
	mtrx_InitR();
}

static void mtrx_setLeds(const LedChainDesc_t *lcd, channel_t *chan, uint8_t r, uint8_t g, uint8_t b)
{
	for (uint8_t i = 0; i < barHeigth; ++i)
	{
		led_setLedToColor(lcd, chan->bar[0].dots[i], r, g, b);
		led_setLedToColor(lcd, chan->bar[1].dots[i], r, g, b);
	}
}

static void mtrx_setLedsScaled(const LedChainDesc_t *lcd, channel_t *chan, uint32_t val, uint8_t r, uint8_t g,
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
		led_setLedToColor(lcd, chan->bar[0].dots[i], r, g, b);
		led_setLedToColor(lcd, chan->bar[1].dots[i], r, g, b);
	}
}

void mtrx_anim(mAnim_t* ctx)
{
	static uint8_t color = 80u;
	led_setAllLedsToColor(ctx->lcd_ctx, 15, 15, 15);
	mtrx_setLedsScaled(ctx->lcd_ctx, &left[0], getLChanVal(e63Hz), 0u, color, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &left[1], getLChanVal(e160Hz), color, 0u, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &left[2], getLChanVal(e400Hz), color, color, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &left[3], getLChanVal(e1kHz), 0u, color, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &left[4], getLChanVal(e2_5kHz), 0u, 0u, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &left[5], getLChanVal(e6_25kHz), color, 0u, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &left[6], getLChanVal(e16kHz), color, color, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lright[0], getRChanVal(e63Hz), 0u, color, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lright[1], getRChanVal(e160Hz), color, 0u, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lright[2], getRChanVal(e400Hz), color, color, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lright[3], getRChanVal(e1kHz), 0u, color, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lright[4], getRChanVal(e2_5kHz), 0u, 0u, color);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lright[5], getRChanVal(e6_25kHz), color, 0u, 0u);
	mtrx_setLedsScaled(ctx->lcd_ctx, &lright[6], getRChanVal(e16kHz), color, color, color);
}
