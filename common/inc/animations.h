/**
 * @file      animations.h
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
 * @brief Public animation functions
 * @defgroup Animations Public interface of animations.
 * @{
 */

#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdint.h>
#include <stdbool.h>
#include "ledData.h"

#define fm_frqBand(name, gf, str, len, rr, gg, bb, bnd, mx)\
    static const frqBand_t name = {.gCv = gf, .pSt = str, .pL = len, .r = rr, .g = gg, .b = bb, .band = bnd, .pM = (str + (len - 1)/2), .hL = (len - 1)/2, .max = mx}

typedef enum  {
	anim_powerUp = 0,

	anim_min = 1,

	anim_rnd3 = 1,
	anim_white,
	anim_red,
	anim_green,
	anim_blue,
	anim_cycleColors,
	anim_layers,
	anim_msqDrv,
	anim_enumAssrt,

	anim_cR1,
	anim_cR2,
	anim_rnd1,
	anim_rnd2,
}anim_mode_e;

typedef enum
{
	init, blueSpawn, greenSpawn, redSpawn, whiteSpawn, fullWhite, done,
} puState_t;

typedef enum
{
	e_render,
	e_waitTxCplt,
	e_paste,
	e_StartDma,
	e_dma
} eSm;

typedef enum
{
	e_fadeIn,
	e_tx,
	e_fadeOut
} dmaState_t;

typedef struct lRawDma_tag
{
	dmaState_t dS;
	uint32_t  i; /*!< index counter for current state */
	const uint32_t ledCount; /*!< count of 'real' leds in the strip */
	const uint32_t rawCount; /*!< half size of dma tx buffer (in units of LedRaw[]) */
	LedRaw* lRaw; /*!< pointer to 'real' raw led ctx */
}lRawDma_t;

typedef struct mAnim_tag mAnim_t;
typedef void (*fpRender)(mAnim_t* ctx);
struct mAnim_tag
{
	fpRender fpRend; /*!< */
	LedChainDesc_t *const lcd_ctx; /*!< */
	const uint32_t triggerTimeMs; /*!< */
	uint32_t lastToggle; /*!< */
	volatile uint32_t sendLock; /*!< */
	volatile uint32_t a, b, c, d, e, f; /*!< */
	eSm state; /*!< */
	puState_t puState; /*!< */
	uint16_t padd2; /*!< */
	anim_mode_e AnimMode;
};

void anim_setCirc(bool shrt);
void anim_CyclicCall(mAnim_t* ctx);
void anim_setMode(LedChainDesc_t *const lcd, anim_mode_e set);
void anim_setBrightness(uint8_t set);
void anim_addBrightness(int8_t add);
void anim_nextMode(LedChainDesc_t *const lcd);

typedef struct rider rider_t;
typedef void (*riderInit)(rider_t*);
struct rider
{
	riderInit fpInit; /*!< */
	uint32_t pos; /*!< */
	fpa_t posIq; /*!< */
	uint32_t posMin; /*!< */
	uint32_t posMax; /*!< */
	fpa_t step; /*!< */
	LedLogic_t c; /*!< */
	uint8_t length; /*!< */
	uint8_t blanks; /*!< */
	uint8_t iteration; /*!< */
	uint16_t padd; /*!< */
};

#if !(defined(STM32F103xB))
void anim_circularRun1(LedChainDesc_t *const lcd, uint8_t brightness);

void anim_initPuRide(LedChainDesc_t *const lcd, rider_t* arg, uint8_t r, uint8_t g, uint8_t b, uint8_t dir);
void rideOnceFiller(LedChainDesc_t *const lcd, rider_t* arg);
void riderBlanker(LedChainDesc_t *const lcd, rider_t* arg);
void riderFiller(LedChainDesc_t *const lcd, rider_t* arg);
void anim_initRedRider(LedChainDesc_t *const lcd, rider_t* arg);
void anim_initRedRider2(LedChainDesc_t *const lcd, rider_t* arg);
void anim_initRedRider3(LedChainDesc_t *const lcd, rider_t* arg);

#else
#define anim_initPuRide(a,b,c,d,e,f)
#define rideOnceFiller(a,b)
#define riderBlanker(a,b)
#define riderFiller(a,b)
#define anim_initRedRider(a,b)
#define anim_initRedRider2(a,b)
#define anim_initRedRider3(a,b)
#define anim_circularRun1(a,b)

#endif

void anim_random3(LedChainDesc_t *const lcd);

#if !( (defined(STM32F303xE) | defined(STM32F103xB)) )
void anim_random1(LedChainDesc_t *const lcd);
void anim_setRandom2CycleCount(uint16_t c);
void anim_random2(LedChainDesc_t *const lcd);
void anim_r23Init(LedChainDesc_t *const lcd);
void anim_r23DeInit(LedChainDesc_t *const lcd);
void anim_layerRedRider(uint32_t pos);

#else
#define anim_random1(x)
#define anim_setRandom2CycleCount(x)
#define anim_random2(x)
#define anim_r23Init(x)

#define anim_r23DeInit(x)
#define anim_layerRedRider(x)

#endif
#include "msgeq7.h"

typedef struct frqBand {
	gChanVal gCv; /*!< */
	uint32_t pSt; /*!< */
	uint32_t pL; /*!< */
	uint32_t pM; /*!< */
	uint32_t hL; /*!< */
	uint32_t max; /*!< */
	uint8_t r; /*!< */
	uint8_t g; /*!< */
	uint8_t b; /*!< */
	msgeq7Freq band; /*!< */
} frqBand_t;

typedef void(*anim_frqDrv_F)(LedChainDesc_t *const lcd, const frqBand_t *fB);

typedef struct frqString {
   anim_frqDrv_F f;
   uint32_t start;
   uint32_t end;
   LedLogic_t backGround;
   const frqBand_t **frqB;
}frqString_t;

extern const frqString_t *frqS[];

void anim_frqDrv(LedChainDesc_t *const lcd, const frqBand_t *fB);
void anim_frqFrvRem(LedChainDesc_t *const lcd, const frqBand_t *fB);
#endif /* ANIMATIONS_H_ */
/** @}*/
