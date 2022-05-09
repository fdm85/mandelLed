/**
 * @file      physic.c
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
 * @brief Physic implementation
 * @ingroup Physic
 * @{
 */


#include "physic.h"
#include "assrt.h"
#include <stdint.h>
#include <stdlib.h>
#include "fpa.h"

/** @brief Check if the given rider's collide
 *  @param a Rider a
 *  @param b Rider b
 *  @retval false the two do NOT collide
 *  @retval true the two DO collide
 */
bool phy_doesCollide(rider_t* a, rider_t* b)
{
	assrt(a);
	assrt(b);
	bool result = false;

	if( (a->pos >= b->pos) && (a->pos <= (b->pos + b->length)) )
	{
		result = true;
	}

	if( ((a->pos + a->length) >= b->pos) && ((a->pos + a->length) <= (b->pos + b->length)) )
	{
		result = true;
	}

	return result;
}

/** @brief Swap colors of the two given rider's
 *  @param a rider a
 *  @param b rider b
 */
void phy_swapColors(rider_t* a, rider_t* b)
{
	uint8_t tr = a->c.r;
	uint8_t tg = a->c.g;
	uint8_t tb = a->c.b;

    a->c.r = b->c.b;
    a->c.g = b->c.r;
    a->c.b = b->c.g;

    b->c.r = tg;
	b->c.g = tb;
	b->c.b = tr;
}

/** @brief Perform simple impact on two rider's
 *  @details Simple impact will simply switch the movement directions of the given rider's
 *  @param a rider a
 *  @param b rider b
 */
void phy_perfSimpleImpact(rider_t* a, rider_t* b)
{
	a->step.r *= -1;
	b->step.r *= -1;
}
/** @}*/
