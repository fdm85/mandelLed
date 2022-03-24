/**
 * @file      physic.h
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
 * @brief Physic public interface
 * @defgroup Physic Simple physic emulation
 * @details Functions to support interaction of dynamic Animations with each other
 * @ingroup Animations
 */

#ifndef PHYSIC_H_
#define PHYSIC_H_

#include <stdbool.h>
#include "animations.h"

#if !(defined(STM32F103xB))
bool phy_doesCollide(rider_t* a, rider_t* b);
void phy_perfSimpleImpact(rider_t* a, rider_t* b);
void phy_swapColors(rider_t* a, rider_t* b);

#else
#define phy_doesCollide(a,b) (false)
#define phy_perfElasticImpact(a,b)
#define phy_perfSimpleImpact(a,b)
#define phy_swapColors(a,b)


#endif
#endif /* PHYSIC_H_ */
