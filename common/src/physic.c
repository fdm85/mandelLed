/*
 * physic.c
 *
 *  Created on: 05.12.2020
 *      Author: cgrue
 */


#include "physic.h"
#include "assrt.h"
#include <stdint.h>
#include <stdlib.h>
#include "fpa.h"

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

void phy_perfSimpleImpact(rider_t* a, rider_t* b)
{
	a->step.r *= -1;
	b->step.r *= -1;
}
