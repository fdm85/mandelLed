/**
 * @file      display.c
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
 * @brief Display emulation for easy configuration
  * @defgroup Display Display emulation for easy configuration
 * @ingroup Matrix
 * @{
 */

#include "display.h"
#include "matrix.h"

/*! @brief User selected value */
static uint16_t setVal = 0u;

static void disp_SelectValue(mAnim_t *ctx) {
   uint16_t curVal = (uint16_t) getEncoder();
   uint8_t color = 20u;
   curVal %= (barLedCount + 1u);
   disp_displayValue16(ctx, curVal, 2u, color, color);

   setVal = curVal;

}

/** @brief main function of display emulation
 *  @param context to work on
 *  @ingroup Display
 */
void disp_display(mAnim_t *ctx) {
   static uint8_t color = 20u;
   static anim_mode_e curMode;
   led_setAllLedsToColor(ctx->lcd_ctx, 2u, 2u, 2u);
   curMode = anim_getCurrMode();

//   switch(state) {
//
//   }
   disp_SelectValue(ctx);
}


static uint8_t getCurVal(void) {
   return setVal;
}

/** @}*/
