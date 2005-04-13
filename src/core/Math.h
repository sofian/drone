/* Math.h
 * Copyright (C) 2004--2005 Jean-Sebastien Senecal
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef MATH_INCLUDED
#define MATH_INCLUDED

#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <limits.h>

//! Basic definitions

#ifndef PI 
#define PI 3.14159265358979323846
#endif

#ifndef HALF_PI
#define HALF_PI 1.57079632679489661923
#endif

#ifndef TWICE_PI
#define TWICE_PI 6.28318530717958623200
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Use template MIN/MAX (safer).

#ifdef MIN
#undef MIN
#endif

#ifdef MAX
#undef MAX
#endif

//! Min.
template <typename T>
T MIN(T x, T y)
{ return (x < y ? x : y); }

//! Max.
template <typename T>
T MAX(T x, T y)
{ return (x > y ? x : y); }

// Begin LIBGIMP

// LIBGIMP - The GIMP Library
// Copyright (c) 2004 Jean-Sébastien Senécal
// Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball

//! Clamp x at lower = l and upper = u.
template <typename T>
inline T CLAMP(T x, T l, T u)
{ return ( x < l ? l : ( x > u ? u : x ) ); }

//! Round.
template <typename T>
inline int ROUND(T x)
{ return (int)(x+0.5); }

//! Square.
template <typename T>
inline T SQR(T x)
{ return (x*x); }

//! Limit a (0->511) int to 255.
inline int MAX255(int a)
{ return (a | ((a & 256) - ((a & 256) >> 8))); }

// End LIBGIMP

//! Clamp a int32-range int between 0 and 255 inclusive
inline unsigned char CLAMP0255(int a)
{
  return (unsigned char)
    ( (((-a) >> 31) & a)  // 0 if the number was negative
      | (255 - a) >> 31); // -1 if the number was greater than 255
}

#define PI_DIV_180 M_PI / 180
inline double DEG2RAD(double d)
{ return (d * PI_DIV_180); }


//! Return whether is outside boundaries (x<l or x>u)
template <typename T>
inline bool IS_OUTSIDE(T x, T l, T u)
{ return ( (x < l) || (x > u) );}

// Fast array operations.

// Additions.

//! Adds #n# elements of #src1# to #src2# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void addVecVec(T *dst, const U *src1, const V *src2, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src1++ + *src2++);
}

//! Adds #n# elements of #src# to #dst#.
template <typename T, typename U>
inline void addAccVecVec(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ += *src++;
}

//! Adds #val# to #n# elements of #src# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void addVecVal(T *dst, const U *src, const V val, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src++ + val);
}

//! Adds #val# to #n# elements of #src#.
template <typename T, typename U>
inline void addAccVecVal(T *src, const U val, size_t n)
{
  while (n--)
    *src++ += val;
}


// Subtractions.

//! Subtracts #n# elements of #src2# from #src1# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void subtractVecVec(T *dst, const U *src1, const V *src2, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src1++ - *src2++);
}

//! Subtracts #n# elements of #src# from #dst#.
template <typename T, typename U>
inline void subtractAccVecVec(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ -= *src++;
}

//! Subtracts #val# from #n# elements of #src# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void subtractVecVal(T *dst, const U *src, const V val, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src++ - val);
}

//! Subtracts #val# to #n# elements of #src#.
template <typename T, typename U>
inline void subtractAccVecVal(T *src, const U val, size_t n)
{
  while (n--)
    *src++ -= val;
}


// Multiplications.

//! Multiplies #n# elements of #src1# to #src2# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void multiplyVecVec(T *dst, const U *src1, const V *src2, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src1++ * *src2++);
}

//! Multiplies #n# elements of #dst# by #src#.
template <typename T, typename U>
inline void multiplyAccVecVec(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ *= *src++;
}

//! Subtracts #val# to #n# elements of #src# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void multiplyVecVal(T *dst, const U *src, const V val, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src++ * val);
}

//! Multiplies #val# with #n# elements of #src#.
template <typename T, typename U>
inline void multiplyAccVecVal(T *src, const U val, size_t n)
{
  while (n--)
    *src++ *= val;
}


// Divisions.

//! Divides #n# elements of #src2# from #src1# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void divideVecVec(T *dst, const U *src1, const V *src2, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src1++ / *src2++);
}

//! Divides #n# elements of #dst# by #src#.
template <typename T, typename U>
inline void divideAccVecVec(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ /= *src++;
}

//! Divides #n# elements of #src# by #val# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void divideVecVal(T *dst, const U *src, const V val, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src++ / val);
}

//! Divides #n# elements of #src# by #val#.
template <typename T, typename U>
inline void divideAccVecVal(T *src, const U val, size_t n)
{
  while (n--)
    *src++ /= val;
}


// Other utility template functions.

//! Copies #n# values of #src# to #dst#. If both are of the same type, better use memcpy(3).
template <typename T, typename U>
inline void copy(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(static_cast<T>(*src++));
}

//! Fills #n# values of #src# with value #val#.
template <typename T, typename U>
inline void fill(T *dst, const U val, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(val);
}

//! Fills #n# values of #src# with zeros.
template <typename T>
inline void clear(T *dst, size_t n)
{
  memset(dst, 0, n*sizeof(T));
}

//! Returns the sum of #n# elements of #src#.
template <typename T>
inline T sum(const T *src, size_t n)
{
  if (!n)
    return 0;
  T ret = *src++;
  while (--n)
    ret += *src++;
  return ret;
}

//! Returns the min element of #n# elements of #src#.
template <typename T>
inline T min(const T *src, size_t n)
{
  if (!n)
    return 0;
  T min = *src++;
  while (--n)
    min = MIN(*src++,min);
  return min;
}

//! Returns the max element of #n# elements of #src#.
template <typename T>
inline T max(const T *src, size_t n)
{
  if (!n)
    return 0;
  T max = *src++;
  while (--n)
    max = MAX(*src++,max);
  return max;
}

//! Fast trigonometric functions approximations

//! Begin WML

// Magic Software, Inc.
// http://www.magic-software.com
// http://www.wild-magic.com
// Copyright (c) 2004.  All Rights Reserved
//
// The Wild Magic Library (WML) source code is supplied under the terms of
// the license agreement http://www.magic-software.com/License/WildMagic.pdf
// and may not be copied or disclosed except in accordance with the terms of
// that agreement.

// #define FAST_MATH_PRECISION 1 // XXX to be changed ultimately (how??)


// // WARNING : these function are only valid in some interval
// // TODO: which interval.. Should be something like -2PI..2PI

// // Fast evaluation of sin(angle) by polynomial approximations.  The angle
// // must be in [0,pi/2].  The maximum absolute error is about 1.7e-04 for
// // FastSin0 and about 2.3e-09 for FastSin1.  The speed up is about 2 for
// // FastSin0 and about 1.5 for FastSin1.
// inline float fastsin0 (float x)
// {
//   float x2 = x*x;
//   float ret = 7.61e-03f;
//   ret *= x2;
//   ret -= 1.6605e-01f;
//   ret *= x2;
//   ret += 1.0f;
//   ret *= x;
//   return ret;
// }

// inline float fastsin1 (float x)
// {
//   float x2 = x*x;
//   float ret = -2.39e-08f;
//   ret *= x2;
//   ret += 2.7526e-06f;
//   ret *= x2;
//   ret -= 1.98409e-04f;
//   ret *= x2;
//   ret += 8.3333315e-03f;
//   ret *= x2;
//   ret -= 1.666666664e-01f;
//   ret *= x2;
//   ret += 1.0f;
//   ret *= x;
//   return ret;
// }

// inline float fastsin (float x)
// {
//   float ret;
//   bool positive = (x >= 0);
//   x = (positive ? x : -x);
//   ret = fmod(x, HALF_PI);
// #if FAST_MATH_PRECISION
//   ret = fastsin1(ret);
// #else
//   ret = fastsin0(ret);
// #endif
//   ret = (positive ^ (fmod(x, TWICE_PI) < PI) ? ret : -ret);
//   return ret;
// }



// // Fast evaluation of cos(angle) by polynomial approximations.  The angle
// // must be in [0,pi/2].  The maximum absolute error is about 1.2e-03 for
// // FastCos0 and about 2.3e-09 for FastCos1.  The speed up is about 2 for
// // FastCos0 and about 1.5 for FastCos1.
// inline float fastcos0 (float x)
// {
//   float x2 = x*x;
//   float ret = 3.705e-02f;
//   ret *= x2;
//   ret -= 4.967e-01f;
//   ret *= x2;
//   ret += 1.0f;
//   return ret;
// }

// inline float fastcos1 (float x)
// {
//   float x2 = x*x;
//   float ret = -2.605e-07f;
//   ret *= x2;
//   ret += 2.47609e-05f;
//   ret *= x2;
//   ret -= 1.3888397e-03f;
//   ret *= x2;
//   ret += 4.16666418e-02f;
//   ret *= x2;
//   ret -= 4.999999963e-01f;
//   ret *= x2;
//   ret += 1.0f;
//   return ret;
// }

// inline float fastcos (float x)
// {
//   float ret;
//   bool positive = (x >= 0);
//   x = (positive ? x : -x);
//   ret = fmod(x, HALF_PI);
// #if FAST_MATH_PRECISION
//   ret = fastsin1(ret);
// #else
//   ret = fastsin0(ret);
// #endif
//   ret = (positive ^ (fmod(x, TWICE_PI) < PI) ? ret : -ret);
//   return ret;
// }



// // Fast evaluation of tan(angle) by polynomial approximations.  The angle
// // must be in [0,pi/4].  The maximum absolute error is about 8.1e-04 for
// // FastTan0 and about 1.9e-08 for FastTan1.  The speed up is about 2.5 for
// // FastTan0 and about 1.75 for FastTan1.
// #if FAST_MATH_PRECISION
// #define fasttan(x) fasttan1(x)
// #else
// #define fasttan(x) fasttan0(x)
// #endif

// inline float fasttan0 (float x)
// {
//   float x2 = x*x;
//   float ret = 2.033e-01f;
//   ret *= x2;
//   ret += 3.1755e-01f;
//   ret *= x2;
//   ret += 1.0f;
//   ret *= x;
//   return ret;
// }

// inline float fasttan1 (float x)
// {
//   float x2 = x*x;
//   float ret = 9.5168091e-03f;
//   ret *= x2;
//   ret += 2.900525e-03f;
//   ret *= x2;
//   ret += 2.45650893e-02f;
//   ret *= x2;
//   ret += 5.33740603e-02f;
//   ret *= x2;
//   ret += 1.333923995e-01f;
//   ret *= x2;
//   ret += 3.333314036e-01f;
//   ret *= x2;
//   ret += 1.0f;
//   ret *= x;
//   return ret;
// }

// // Fast evaluation of asin(value) by a sqrt times a polynomial.  The value
// // must be in [0,1].  The maximum absolute error is about 6.8e-05 and the
// // speed up is about 2.5.
// inline float fastasin (float x)
// {
//   float xRoot = sqrt(1.0f-x);
//   float ret = -0.0187293f;
//   ret *= x;
//   ret += 0.0742610f;
//   ret *= x;
//   ret -= 0.2121144f;
//   ret *= x;
//   ret += 1.5707288f;
//   ret = HALF_PI - xRoot*ret;
//   return ret;
// }

// // Fast evaluation of acos(value) by a sqrt times a polynomial.  The value
// // must be in [0,1].  The maximum absolute error is about 6.8e-05 and the
// // speed up is about 2.5.
// inline float fastacos (float x)
// {
//   float xRoot = sqrt(1.0f-x);
//   float ret = -0.0187293f;
//   ret *= x;
//   ret += 0.0742610f;
//   ret *= x;
//   ret -= 0.2121144f;
//   ret *= x;
//   ret += 1.5707288f;
//   ret *= xRoot;
//   return ret;
// }

// #if FAST_MATH_PRECISION
// #define fastatan(x) fastatan1(x)
// #else
// #define fastatan(x) fastatan0(x)
// #endif

// inline float fastatan0 (float x)
// {
//   float x2 = x*x;
//   float ret = 0.0208351f;
//   ret *= x2;
//   ret -= 0.085133f;
//   ret *= x2;
//   ret += 0.180141f;
//   ret *= x2;
//   ret -= 0.3302995f;
//   ret *= x2;
//   ret += 0.999866f;
//   ret *= x;
//   return ret;
// }

// inline float fastatan1 (float x)
// {
//   float x2 = x*x;
//   float ret = 0.0028662257f;
//   ret *= x2;
//   ret -= 0.0161657367f;
//   ret *= x2;
//   ret += 0.0429096138f;
//   ret *= x2;
//   ret -= 0.0752896400f;
//   ret *= x2;
//   ret += 0.1065626393f;
//   ret *= x2;
//   ret -= 0.1420889944f;
//   ret *= x2;
//   ret += 0.1999355085f;
//   ret *= x2;
//   ret -= 0.3333314528f;
//   ret *= x2;
//   ret += 1.0f;
//   ret *= x;
//   return ret;
// }

//! End WML

//
inline float fastDist(float x, float y)
{
  // well... not that fast yet :)
  return sqrt( x*x + y*y );
}

inline float fastAngle(float x, float y)
{
  // well... not that fast yet :)
  return atan2f(y,x);
}

extern void initMath();

// Begin fast trigonometric functions.

// This implementation of fast trigonometric functions was taken from
// the source of Fluxus and is distributed under the GPL.
// (c) 2005 David Griffiths <dave@pawfal.org>

const int SINCOS_TABLESIZE = 2048;
extern float sinLut[SINCOS_TABLESIZE];
extern float cosLut[SINCOS_TABLESIZE];
extern float SINCOS_LOOKUP;

inline float fastsin(float a)
{
  return sinLut[ (int)rint(a*SINCOS_LOOKUP)&SINCOS_TABLESIZE-1 ];
}

inline float fastcos(float a)
{
  return cosLut[ (int)rint(a*SINCOS_LOOKUP)&SINCOS_TABLESIZE-1 ];
}

// End fast trigonometric functions.

// // warning : theta must be clamped inside some range 0..2PI (see fastcos)
// inline float fastPolarToX(float rho, float theta)
// {
//   return fastcos(theta)*rho;
// }

// // warning : theta must be clamped inside some range 0..2PI (see fastsin)
// inline float fastPolarToY(float rho, float theta)
// {
//   return fastsin(theta)*rho;
// }

//! clamps the value in a mirror fashion :
//! It is like the interval was infinite, but repeating itself 'mirrorly'
//! e.g: MIRROR_CLAMP(x,0,2) for x={-3,-2,-1,0,1,2,3} gives :
//! {1,2,1,0,1,2,1}
//! WARNING : not thread safe because of static variables

template<class T>
T MIRROR_CLAMP(T t,T low,T high)
{
  if(t>=low && t<=high)
    return t;
  else
  {
    static T range,howmuch;
    static int howmuchtimes,flip;
    range=high-low;
    if(t<low)
    {
      howmuch=low-t;
      flip=0;
    }
    else
    {
      howmuch=t-high;
      flip=1;
    }
    howmuchtimes = (int)(howmuch/range);
    if((howmuchtimes+flip)%2)
      return high-howmuch+howmuchtimes*range;
    else
      return low+howmuch-howmuchtimes*range;
  }
}

//! clamps the value in a repeating fashion :
//! It is like the interval was infinite, but repeating itself *not* 'mirrorly'
//! e.g: REPEAT_CLAMP(x,0,2) for x={-3,-2,-1,0,1,2,3} gives :
//! {0,1,2,0,1,2,0}

template<class T>
T REPEAT_CLAMP(T t,T low,T high)
{
  ++high; // high = high+1
  T range = high-low;
  while (t < low)
    t += range;
  while (t >= high)
    t -= range;
  return t;
}

float uniform_bounded(float a,float b);

#endif
