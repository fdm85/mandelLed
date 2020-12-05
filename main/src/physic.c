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

//        (m1 * v1) + m2 * (2 * v2 - v1)
// v1' = ---------------------------------
//                    m1 + m2

//        (m2 * v2) + m1 * (2 * v1 - v2)
// v2' = ---------------------------------
//                    m1 + m2
void phy_perfElasticImpact(rider_t* a, rider_t* b)
{
	int32_t m12 = (int32_t)a->length + (int32_t)b->length;
	int32_t v1 = a->step * (a->sig ? -1 : 1);
	int32_t v2 = a->step * (b->sig ? -1 : 1);


	int32_t v1_N = ((int32_t)a->length * v1) + ( (int32_t)b->length * (2 * v2 - v1) );
	int32_t v2_N = ((int32_t)b->length * v2) + ( (int32_t)a->length * (2 * v1 - v2) );

	int32_t v1_  = v1_N / m12;
	int32_t v2_  = v2_N / m12;


	assrt(abs(v1_) <= UINT8_MAX);
	assrt(abs(v2_) <= UINT8_MAX);

	a->step = (uint8_t)abs(v1_);
	b->step = (uint8_t)abs(v2_);

	a->sig = (v1_ < 0L) ? true : false;
	b->sig = (v2_ < 0L) ? true : false;


	uint8_t tr = a->c.r;
	uint8_t tg = a->c.g;
	uint8_t tb = a->c.b;

    a->c.r = b->c.b;
    a->c.g = b->c.r;
    a->c.b = b->c.g;

    b->c.r = tb;
	b->c.g = tr;
	b->c.b = tg;
}
