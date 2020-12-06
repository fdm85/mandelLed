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

bool phy_doesCollide(rider_t* a, rider_t* b);
void phy_perfElasticImpact(rider_t* a, rider_t* b);
void phy_perfSimpleImpact(rider_t* a, rider_t* b);
void phy_swapColors(rider_t* a, rider_t* b);
#endif /* PHYSIC_H_ */
