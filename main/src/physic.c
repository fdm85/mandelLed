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
#include "IQmathLib.h"

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
	a->step *= -1;
	b->step *= -1;

	phy_swapColors(a, b);
}

//        (m1 * v1) + m2 * (2 * v2 - v1)
// v1' = ---------------------------------
//                    m1 + m2

//        (m2 * v2) + m1 * (2 * v1 - v2)
// v2' = ---------------------------------
//                    m1 + m2
void phy_perfElasticImpact(rider_t* a, rider_t* b)
{
	_iq m12 = _IQG((int32_t)a->length + (int32_t)b->length);
	_iq v1 = a->step;
	_iq v2 = b->step;
	_iq m1 = _IQ(a->length);
	_iq m2 = _IQ(b->length);


	_iq v1_N = ( _IQmpy(m1, v1) + _IQmpy(m2, (_IQmpy2(v2) - v1)) );
	_iq v2_N = ( _IQmpy(m2, v2) + _IQmpy(m1, (_IQmpy2(v1) - v1)) );

	_iq v1_  = _IQdiv(v1_N, m12);
	_iq v2_  = _IQdiv(v2_N, m12);


	assrt(_IQint(_IQabs(v1_)) <= UINT8_MAX);
	assrt(_IQint(_IQabs(v2_)) <= UINT8_MAX);

	a->step = v1_;
	b->step = v2_;

	phy_swapColors(a, b);
}
