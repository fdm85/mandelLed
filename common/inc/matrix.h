/**
 * @file      matrix.h
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
 * @brief Matrix Interface
 * @defgroup Matrix Matrix based Animations
 * @{
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#define barHeigth 8
#define barWidth 2
#define barLedCount (barHeigth*barWidth)
#define channelWidth 7
#define auxChanWidth 1

#include <stdint.h>
#include "ledData.h"
#include "animations.h"

void mtrx_Init(void);
void mtrx_anim(mAnim_t* ctx);
void mtrx_display(mAnim_t *ctx);
#endif /* MATRIX_H_ */
/** @}*/
