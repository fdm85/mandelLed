/**
 * @file      fpa.h
 * @authors   Clemens Grünberger
 * @copyright 2021  Clemens Grünberger
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
 * @brief fixed point arithmetic
 * @defgroup fpa fixed point arithmetic
 * @ingroup math Math and helper functions
 * @details Header for fpa use. Fixed point arithmetic shall bring easy and
 * performant workaround to floating point arithmetic. Focus here is to use fpa
 * while retaining the ability to easily read the integer parts to ease debugging.
 */

#ifndef INC_FPA_H_
#define INC_FPA_H_

#include <stdint.h>

// ToDo add fancy switches to change base-type

#ifdef FPA_FRAC
	#define Sz	(32)
	#define iSz (Sz-FPA_FRAC)
	typedef int32_t fpaT;
	typedef uint32_t fpaTf;
#else
	#pragma warning fixed point arithmetic not defined
#endif

typedef union fpa_tag {
	fpaT r;
	struct {
		fpaTf f:FPA_FRAC;
		fpaT i:iSz;
	};
} fpa_t;

static inline fpa_t FPA_mult(fpa_t m1, fpa_t m2) {
	int64_t t = m1.r * m2.r;
	return (fpa_t)((fpaT)(t >> FPA_FRAC));
}

static inline fpa_t FPA_div(fpa_t d, fpa_t s) {
	int64_t t = ((int64_t)(d.r) << FPA_FRAC);
	return (fpa_t)((fpaT)(t / s.r));
}

static inline fpa_t FPA_IntDivFpa(int64_t d, fpa_t s) {
	int64_t t = d << (2*FPA_FRAC);
	return (fpa_t)((fpaT)(t / s.r));
}

#endif /* INC_FPA_H_ */
