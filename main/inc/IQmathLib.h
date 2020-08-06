//-----------------------------------------------------------------------------
//! \addtogroup     IQmathLib
//! @{
//! \brief          IQ math lib
//! \details        calculation macros for IQ math
//! \copyright      <a href="http://www.feo-elektronik.de">Feo Elektronik GmbH</a>,
//!                 Zwergerstr. 15, 88214 Ravensburg

//-----------------------------------------------------------------------------
#ifndef __IQMATHLIB_H__
#define __IQMATHLIB_H__

#include <stdint.h>

//*****************************************************************************
//
// By redefining MATH_TYPE, all IQmath functions will be replaced by their
// floating point equivalents.
//
//*****************************************************************************
#define FLOAT_MATH              0   //!< use float for IQmath
#define IQ_MATH                 1   //!< use fixed point integer for IQmath
#ifndef MATH_TYPE
#define MATH_TYPE               IQ_MATH
#endif

//*****************************************************************************
//
// The IQ format to be used when the IQ format is not explicitly specified
// (such as _IQcos instead of _IQ16cos).  This value must be between 1 and 30,
// inclusive.
//
//*****************************************************************************
#ifndef GLOBAL_IQ
	#error global IQ not defined
#endif

//*****************************************************************************
//
// Include some standard headers, as required based on the math type.
//
//*****************************************************************************
#if MATH_TYPE == FLOAT_MATH
#include <math.h>
#endif
#include <limits.h>
#if !_TARGET_PC
   #define __STATIC_INLINE  static inline
#else
   #define __STATIC_INLINE  static __inline
#endif


//*****************************************************************************
//
// See if IQmath or floating point is being used.
//
//*****************************************************************************
#if MATH_TYPE == IQ_MATH

//*****************************************************************************
//
// The types for the various Q formats.
//
//*****************************************************************************
typedef int32_t _iq24;
typedef int32_t _iq23;
typedef int32_t _iq22;
typedef int32_t _iq21;
typedef int32_t _iq20;
typedef int32_t _iq19;
typedef int32_t _iq18;
typedef int32_t _iq17;
typedef int32_t _iq16;
typedef int32_t _iq15;
typedef int32_t _iq14;
typedef int32_t _iq13;
typedef int32_t _iq12;
typedef int32_t _iq11;
typedef int32_t _iq10;
typedef int32_t _iq;
typedef _iq (*fpMultIQNum) (_iq A, _iq B);
//*****************************************************************************
//
// Simple multiplies or divides, which are accomplished with simple shifts.
//
//*****************************************************************************
#define _IQmpy2(A)             ((A) << 1)   //!<
#define _IQmpy4(A)             ((A) << 2)   //!<
#define _IQmpy8(A)             ((A) << 3)   //!<
#define _IQmpy16(A)            ((A) << 4)   //!<
#define _IQmpy32(A)            ((A) << 5)   //!<
#define _IQmpy64(A)            ((A) << 6)   //!<
#define _IQdiv2(A)             ((A) >> 1)   //!<
#define _IQdiv4(A)             ((A) >> 2)   //!<
#define _IQdiv8(A)             ((A) >> 3)   //!<
#define _IQdiv16(A)            ((A) >> 4)   //!<
#define _IQdiv32(A)            ((A) >> 5)   //!<
#define _IQdiv64(A)            ((A) >> 6)   //!<

//*****************************************************************************
//
// Convert a value (int or float) into an IQ number.
//
//*****************************************************************************
#define _IQ24(A)               ((_iq24)((A) * (1 << 24)))   //!<
#define _IQ23(A)               ((_iq23)((A) * (1 << 23)))   //!<
#define _IQ22(A)               ((_iq22)((A) * (1 << 22)))   //!<
#define _IQ21(A)               ((_iq21)((A) * (1 << 21)))   //!<
#define _IQ20(A)               ((_iq20)((A) * (1 << 20)))   //!<
#define _IQ19(A)               ((_iq19)((A) * (1 << 19)))   //!<
#define _IQ18(A)               ((_iq18)((A) * (1 << 18)))   //!<
#define _IQ17(A)               ((_iq17)((A) * (1 << 17)))   //!<
#define _IQ16(A)               ((_iq16)((A) * (1 << 16)))   //!<
#define _IQ15(A)               ((_iq15)((A) * (1 << 15)))   //!<
#define _IQ14(A)               ((_iq14)((A) * (1 << 14)))   //!<
#define _IQ13(A)               ((_iq13)((A) * (1 << 13)))   //!<
#define _IQ12(A)               ((_iq12)((A) * (1 << 12)))   //!<
#define _IQ11(A)               ((_iq11)((A) * (1 << 11)))   //!<
#define _IQ10(A)               ((_iq10)((A) * (1 << 10)))   //!<
#define _IQ9(A)                ((_iq9) ((A) * (1 << 9)))    //!<
#define _IQG(A)                ((_iq9) ((A) * (1 << GLOBAL_IQ)))    //!<

#if GLOBAL_IQ == 24
 #define _IQ(A)                  _IQ24(A)
#elif GLOBAL_IQ == 23
 #define _IQ(A)                  _IQ23(A)
#elif GLOBAL_IQ == 22
 #define _IQ(A)                  _IQ22(A)
#elif GLOBAL_IQ == 21
 #define _IQ(A)                  _IQ21(A)
#elif GLOBAL_IQ == 20
 #define _IQ(A)                  _IQ20(A)
#elif GLOBAL_IQ == 19
 #define _IQ(A)                  _IQ19(A)
#elif GLOBAL_IQ == 18
 #define _IQ(A)                  _IQ18(A)
#elif GLOBAL_IQ == 17
 #define _IQ(A)                  _IQ17(A)
#elif GLOBAL_IQ == 16
 #define _IQ(A)                  _IQ16(A)
#elif GLOBAL_IQ == 15
 #define _IQ(A)                  _IQ15(A)
#elif GLOBAL_IQ == 14
 #define _IQ(A)                  _IQ14(A)
#elif GLOBAL_IQ == 13
 #define _IQ(A)                  _IQ13(A)
#elif GLOBAL_IQ == 12
 #define _IQ(A)                  _IQ12(A)
#elif GLOBAL_IQ == 11
 #define _IQ(A)                  _IQ11(A)
#elif GLOBAL_IQ == 10
 #define _IQ(A)                  _IQ10(A)
#else
 #define _IQ(A)                  _IQG(A)
#endif


//*****************************************************************************
//
// Converts a number between IQ format and 16-bit Qn format.
//
//*****************************************************************************
#if (GLOBAL_IQ >= 15)
#define _IQtoQ15(A)             ((int32_t)(A) >> (GLOBAL_IQ - 15))
#define _Q15toIQ(A)             ((_iq15)(A) << (GLOBAL_IQ - 15))
#else
#define _IQtoQ15(A)             ((int32_t)(A) << (15 - GLOBAL_IQ))
#define _Q15toIQ(A)             ((_iq15)(A) >> (15 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 14)
#define _IQtoQ14(A)             ((int32_t)(A) >> (GLOBAL_IQ - 14))
#define _Q14toIQ(A)             ((_iq14)(A) << (GLOBAL_IQ - 14))
#else
#define _IQtoQ14(A)             ((int32_t)(A) << (14 - GLOBAL_IQ))
#define _Q14toIQ(A)             ((_iq14)(A) >> (14 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 13)
#define _IQtoQ13(A)             ((int32_t)(A) >> (GLOBAL_IQ - 13))
#define _Q13toIQ(A)             ((_iq13)(A) << (GLOBAL_IQ - 13))
#else
#define _IQtoQ13(A)             ((int32_t)(A) << (13 - GLOBAL_IQ))
#define _Q13toIQ(A)             ((_iq13)(A) >> (13 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 12)
#define _IQtoQ12(A)             ((int32_t)(A) >> (GLOBAL_IQ - 12))
#define _Q12toIQ(A)             ((_iq12)(A) << (GLOBAL_IQ - 12))
#else
#define _IQtoQ12(A)             ((int32_t)(A) << (12 - GLOBAL_IQ))
#define _Q12toIQ(A)             ((_iq12)(A) >> (12 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 11)
#define _IQtoQ11(A)             ((int32_t)(A) >> (GLOBAL_IQ - 11))
#define _Q11toIQ(A)             ((_iq11)(A) << (GLOBAL_IQ - 11))
#else
#define _IQtoQ11(A)             ((int32_t)(A) << (11 - GLOBAL_IQ))
#define _Q11toIQ(A)             ((_iq11)(A) >> (11 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 10)
#define _IQtoQ10(A)             ((int32_t)(A) >> (GLOBAL_IQ - 10))
#define _Q10toIQ(A)             ((_iq10)(A) << (GLOBAL_IQ - 10))
#else
#define _IQtoQ10(A)             ((int32_t)(A) << (10 - GLOBAL_IQ))
#define _Q10toIQ(A)             ((_iq10)(A) >> (10 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 9)
#define _IQtoQ9(A)              ((int32_t)(A) >> (GLOBAL_IQ - 9))
#define _Q9toIQ(A)              ((_iq9)(A) << (GLOBAL_IQ - 9))
#else
#define _IQtoQ9(A)              ((int32_t)(A) << (9 - GLOBAL_IQ))
#define _Q9toIQ(A)              ((_iq9)(A) >> (9 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 8)
#define _IQtoQ8(A)              ((int32_t)(A) >> (GLOBAL_IQ - 8))
#define _Q8toIQ(A)              ((_iq8)(A) << (GLOBAL_IQ - 8))
#else
#define _IQtoQ8(A)              ((int32_t)(A) << (8 - GLOBAL_IQ))
#define _Q8toIQ(A)              ((_iq8)(A) >> (8 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 7)
#define _IQtoQ7(A)              ((int32_t)(A) >> (GLOBAL_IQ - 7))
#define _Q7toIQ(A)              ((_iq7)(A) << (GLOBAL_IQ - 7))
#else
#define _IQtoQ7(A)              ((int32_t)(A) << (7 - GLOBAL_IQ))
#define _Q7toIQ(A)              ((_iq7)(A) >> (7 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 6)
#define _IQtoQ6(A)              ((int32_t)(A) >> (GLOBAL_IQ - 6))
#define _Q6toIQ(A)              ((_iq6)(A) << (GLOBAL_IQ - 6))
#else
#define _IQtoQ6(A)              ((int32_t)(A) << (6 - GLOBAL_IQ))
#define _Q6toIQ(A)              ((_iq6)(A) >> (6 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 5)
#define _IQtoQ5(A)              ((int32_t)(A) >> (GLOBAL_IQ - 5))
#define _Q5toIQ(A)              ((_iq5)(A) << (GLOBAL_IQ - 5))
#else
#define _IQtoQ5(A)              ((int32_t)(A) << (5 - GLOBAL_IQ))
#define _Q5toIQ(A)              ((_iq5)(A) >> (5 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 4)
#define _IQtoQ4(A)              ((int32_t)(A) >> (GLOBAL_IQ - 4))
#define _Q4toIQ(A)              ((_iq4)(A) << (GLOBAL_IQ - 4))
#else
#define _IQtoQ4(A)              ((int32_t)(A) << (4 - GLOBAL_IQ))
#define _Q4toIQ(A)              ((_iq4)(A) >> (4 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 3)
#define _IQtoQ3(A)              ((int32_t)(A) >> (GLOBAL_IQ - 3))
#define _Q3toIQ(A)              ((_iq3)(A) << (GLOBAL_IQ - 3))
#else
#define _IQtoQ3(A)              ((int32_t)(A) << (3 - GLOBAL_IQ))
#define _Q3toIQ(A)              ((_iq3)(A) >> (3 - GLOBAL_IQ))
#endif

#if (GLOBAL_IQ >= 2)
#define _IQtoQ2(A)              ((int32_t)(A) >> (GLOBAL_IQ - 2))
#define _Q2toIQ(A)              ((_iq2)(A) << (GLOBAL_IQ - 2))
#else
#define _IQtoQ2(A)              ((int32_t)(A) << (2 - GLOBAL_IQ))
#define _Q2toIQ(A)              ((_iq2)(A) >> (2 - GLOBAL_IQ))
#endif

#define _IQtoQ1(A)              ((int32_t)(A) >> (GLOBAL_IQ - 1))
#define _Q1toIQ(A)              ((_iq1)(A) << (GLOBAL_IQ - 1))


//*****************************************************************************
//
// Convert an IQ number to a floating point value.
//
//*****************************************************************************
__STATIC_INLINE float _IQ16toF(_iq16 A) { return (float)A * (1.0f/65536.0f); }  //!<
__STATIC_INLINE float _IQ15toF(_iq15 A) { return (float)A * (1.0f/32768.0f); }  //!<
__STATIC_INLINE float _IQ14toF(_iq14 A) { return (float)A * (1.0f/16384.0f); }  //!<
__STATIC_INLINE float _IQ13toF(_iq13 A) { return (float)A * (1.0f/ 8192.0f); }  //!<
__STATIC_INLINE float _IQ12toF(_iq12 A) { return (float)A * (1.0f/ 4096.0f); }  //!<
__STATIC_INLINE float _IQ11toF(_iq11 A) { return (float)A * (1.0f/ 2048.0f); }  //!<
__STATIC_INLINE float _IQ10toF(_iq10 A) { return (float)A * (1.0f/ 1024.0f); }  //!<

#if GLOBAL_IQ == 16
 #define _IQtoF(A)              _IQ16toF(A)
#elif GLOBAL_IQ == 15
 #define _IQtoF(A)              _IQ15toF(A)
#elif GLOBAL_IQ == 14
 #define _IQtoF(A)              _IQ14toF(A)
#elif GLOBAL_IQ == 13
 #define _IQtoF(A)              _IQ13toF(A)
#elif GLOBAL_IQ == 12
 #define _IQtoF(A)              _IQ12toF(A)
#endif

//*****************************************************************************
//
// Saturates an IQ number in a given range.
//
//*****************************************************************************
__STATIC_INLINE _iq _IQsat(_iq A, _iq Pos, _iq Neg) { return ((A) > (Pos)) ? (Pos) : (((A) < (Neg)) ? (Neg) : (A)); }


//*****************************************************************************
//
// Multiplies two IQ numbers.
//
//*****************************************************************************
__STATIC_INLINE _iq16 _IQ16mpy(_iq16 A, _iq16 B) { return (_iq16) (((int64_t)(A) * (B)) >> 16); }   //!<
__STATIC_INLINE _iq15 _IQ15mpy(_iq15 A, _iq15 B) { return (_iq15) (((int64_t)(A) * (B)) >> 15); }   //!<
__STATIC_INLINE _iq14 _IQ14mpy(_iq14 A, _iq14 B) { return (_iq14) (((int64_t)(A) * (B)) >> 14); }   //!<
__STATIC_INLINE _iq13 _IQ13mpy(_iq13 A, _iq13 B) { return (_iq13) (((int64_t)(A) * (B)) >> 13); }   //!<
__STATIC_INLINE _iq12 _IQ12mpy(_iq12 A, _iq12 B) { return (_iq12) (((int64_t)(A) * (B)) >> 12); }   //!<
__STATIC_INLINE _iq11 _IQ11mpy(_iq11 A, _iq11 B) { return (_iq11) (((int64_t)(A) * (B)) >> 11); }   //!<
__STATIC_INLINE _iq10 _IQ10mpy(_iq10 A, _iq10 B) { return (_iq10) (((int64_t)(A) * (B)) >> 10); }   //!<
__STATIC_INLINE _iq   _IQmpy(_iq A, _iq B)       { return (_iq)   (((int64_t)A * B) >> GLOBAL_IQ); } //!<

//*****************************************************************************
//
// Divides two IQ numbers. Can also divide two int numbers and return IQ.
//
//*****************************************************************************
__STATIC_INLINE _iq16 _IQ16div(_iq16 A, _iq16 B) { return (_iq16)(((int64_t)(A) << 16) / (B)); }    //!<
__STATIC_INLINE _iq15 _IQ15div(_iq15 A, _iq15 B) { return (_iq15)(((int64_t)(A) << 15) / (B)); }    //!<
__STATIC_INLINE _iq14 _IQ14div(_iq14 A, _iq14 B) { return (_iq14)(((int64_t)(A) << 14) / (B)); }    //!<
__STATIC_INLINE _iq13 _IQ13div(_iq13 A, _iq13 B) { return (_iq13)(((int64_t)(A) << 13) / (B)); }    //!<
__STATIC_INLINE _iq12 _IQ12div(_iq12 A, _iq12 B) { return (_iq12)(((int64_t)(A) << 12) / (B)); }    //!<
__STATIC_INLINE _iq11 _IQ11div(_iq11 A, _iq11 B) { return (_iq11)(((int64_t)(A) << 11) / (B)); }    //!<
__STATIC_INLINE _iq10 _IQ10div(_iq10 A, _iq10 B) { return (_iq10)(((int64_t)(A) << 10) / (B)); }    //!<
__STATIC_INLINE _iq   _IQdiv(_iq A, _iq B)       { return (_iq)(((int64_t)(A) << GLOBAL_IQ) / (B)); } //!<


//*****************************************************************************
//
// Returns the integer portion of an IQ number.
//
//*****************************************************************************
#define _IQ16int(A)             ((A) >> 16)         //!<
#define _IQ15int(A)             ((A) >> 15)         //!<
#define _IQ14int(A)             ((A) >> 14)         //!<
#define _IQ13int(A)             ((A) >> 13)         //!<
#define _IQ12int(A)             ((A) >> 12)         //!<
#define _IQ11int(A)             ((A) >> 11)         //!<
#define _IQ10int(A)             ((A) >> 10)         //!<
#define _IQint(A)               ((A) >> GLOBAL_IQ)  //!<


//*****************************************************************************
//! \brief   Multiplies two numbers of any IQ format, returning the result in any IQ format
//! \param   S: = ((IQ_A + IQ_B) - IQ_Result)
//! \date    2019-12-05 (WR) - bugfix, int32_t tmp -> int64_t
//*****************************************************************************
__STATIC_INLINE int32_t __IQxmpy(long A, long B, long S) { int64_t tmp = (int64_t)A * B; return ((S < 0) ? (((int32_t)tmp) << -S) : (int32_t)(tmp >> S)); }


//*****************************************************************************
//
// Computes the absolute value of an IQ number.
//
//*****************************************************************************
//#define _IQabs(A)               labs(A)
#define _IQabs(A)               (((A) < 0) ? - (A) : (A))

//*****************************************************************************
//
// Otherwise, floating point math is being used.
//
//*****************************************************************************
#else // MATH_TYPE == FLOAT_MATH

#error "FLOAT_MATH not supported"

#endif // MATH_TYPE == IQ_MATH

//! @}

#endif // __IQMATHLIB_H__
