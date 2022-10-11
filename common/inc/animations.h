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

/*! @brief factory macro to generate frqBand related led-bar */
#define fm_frqBand(name, gf, str, len, rr, gg, bb, bnd, mx)\
    static const frqBand_t name = {.gCv = gf, .pSt = str, .pL = len, .r = rr, .g = gg, .b = bb, .band = bnd, .pM = (str + (len - 1)/2), .hL = (len - 1)/2, .max = mx}

/*! @brief animation mode(s) */
typedef enum  {
	anim_powerUp = 0, /*!< power up sequence */

	anim_min = 1, /*!< helper enum, surrogate of first "real" animation */

	anim_rnd3 = 1, /*!< random 3 visualization */
	anim_msqDrv, /*!< music driven visualization */
	anim_blue, /*!< only blue */
	anim_white, /*!< only white */
	anim_red, /*!< only red */
	anim_green, /*!< only green */
	anim_cycleColors, /*!< epileptic's nightmare */
	anim_layers, /*!< layerd animation WIP */
	anim_enumAssrt, /*!< helper enum to wrap around when iterating through visualization mode's */

	// obsolete or outdated
	anim_cR1,
	anim_cR2,
	anim_rnd1,
	anim_rnd2,
}anim_mode_e;

/*! @brief power up animation states */
typedef enum
{
	init, /*! @brief init sate */
	blueSpawn, /*!< spwan blue rider */
	greenSpawn, /*!< spawn green rider */
	redSpawn, /*!< spawn red rider*/
	whiteSpawn, /*!< spawn white rider */
	fullWhite, /*!< transit to white */
	done, /*!< end state, switch to selected visualization */
} puState_t;

/*! @brief possible working states */
typedef enum
{
	e_render, /*!< render next */
	e_waitTxCplt, /*!< wait until transmission is concluded */
	e_paste /*!< paste from buffer to DMA storage */
} eSm;

typedef struct mAnim_tag mAnim_t;
typedef void (*fpRender)(mAnim_t* ctx);
struct mAnim_tag
{
	fpRender fpRend; /*!< current render function */
	LedChainDesc_t *const lcd_ctx; /*!< led chain context to work on */
	const uint32_t triggerTimeMs; /*!< cycle-time set value */
	uint32_t lastToggle; /*!< last render start */
	volatile uint32_t sendLock; /*!< DMA tx helper semaphore */
	volatile uint32_t a, b, c, d, e, f; /*!< RTM helper counters */
	eSm state; /*!< current render state */
	puState_t puState; /*!< power up state */
	uint16_t padd2; /*!< alignment padding */
};

void anim_setCirc(bool shrt);
void anim_CyclicCall(mAnim_t* ctx);
void anim_setMode(LedChainDesc_t *const lcd, anim_mode_e set);
anim_mode_e anim_getCurrMode(void);
void anim_setBrightness(uint8_t set);
void anim_addBrightness(int8_t add);
void anim_nextMode(LedChainDesc_t *const lcd);

/*! @brief forward declaration of rider struct */
typedef struct rider rider_t;
/*! @brief forward declaration of rider init function */
typedef void (*riderInit)(rider_t*);
/*! @brief rider struct
 * @details A rider struct is a contiguous set of led's displaying in the same color.
 * It may move in a defined area with defined speed.
 *
 * */
struct rider
{
	riderInit fpInit; /*!< rider init function */
	uint32_t pos; /*!< current position as int */
	fpa_t posIq; /*!< current position as fixed-point */
	uint32_t posMin; /*!< lowest possible position */
	uint32_t posMax; /*!< highest possible position */
	fpa_t step; /*!< step per cycle / speed */
	LedLogic_t c; /*!< color of rider */
	uint8_t length; /*!< length of rider in led's */
	uint8_t blanks; /*!< blanks at both ends */
	uint8_t iteration; /*!< iteration count */
	uint16_t padd; /*!< alignment */
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


#if !( (defined(STM32F303xE) | defined(STM32F103xB)) )
void anim_random1(LedChainDesc_t *const lcd);
void anim_setRandom2CycleCount(uint16_t c);
void anim_random2(LedChainDesc_t *const lcd);
void anim_random3(LedChainDesc_t *const lcd);
void anim_r23Init(LedChainDesc_t *const lcd);
void anim_r23DeInit(LedChainDesc_t *const lcd);
void anim_layerRedRider(uint32_t pos);

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
#else
#define anim_random1(x)
#define anim_setRandom2CycleCount(x)
#define anim_random2(x)
#define anim_random3(x)
#define anim_r23Init(x)

#define anim_r23DeInit(x)
#define anim_layerRedRider(x)

#endif

#endif /* ANIMATIONS_H_ */
/** @}*/
