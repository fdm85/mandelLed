/*
 * physic.h
 *
 *  Created on: 05.12.2020
 *      Author: cgrue
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
