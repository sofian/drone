/* DroneMath.h
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

#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <limits.h>
#include <float.h>

//! Init function.
extern void initMath();

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
// Copyright (c) 2004 Jean-S�bastien Sen�cal : changed macros to template functions
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

//! clamps the value in a mirror fashion :
//! It is like the interval was infinite, but repeating itself 'mirrorly'
//! e.g: MIRROR_CLAMP(x,0,2) for x={-3,-2,-1,0,1,2,3} gives :
//! {1,2,1,0,1,2,1}
template<class T>
T MIRROR_CLAMP(T t,T low,T high)
{
  T range,howmuch;
  int howmuchtimes,flip;

  if(t>=low && t<=high)
    return t;
  else
  {
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
  T range;
  ++high; // high = high+1
  range = high-low;
  while (t < low)
    t += range;
  while (t >= high)
    t -= range;
  return t;
}

#define PI_DIV_180 PI / 180
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

// Begin fast trigonometric functions.

// This implementation of fast trigonometric functions was taken from
// the source of Fluxus and is distributed under the GPL.
// (c) 2005 David Griffiths <dave@pawfal.org>

#define FASTMATH_TABLESIZE 2048
#define FASTMATH_TABLE_MASK 2047 // == FASTMATH_TABLESIZE - 1
extern float sinLut[FASTMATH_TABLESIZE];
extern float cosLut[FASTMATH_TABLESIZE];
extern float sqrtLut[FASTMATH_TABLESIZE];
extern float FASTMATH_LOOKUP;

inline float fastsin(float a)
{
  return sinLut[ (int)rint(a*FASTMATH_LOOKUP) & FASTMATH_TABLE_MASK ];
}

inline float fastcos(float a)
{
  return cosLut[ (int)rint(a*FASTMATH_LOOKUP) & FASTMATH_TABLE_MASK ];
}

inline float fastsqrt(float a)
{
  return sqrtLut[ (int)rint(a*FASTMATH_LOOKUP) & FASTMATH_TABLE_MASK ];
}

inline unsigned int ceilingPowerOfTwo(unsigned int x)
{
	unsigned int result = x - 1;
	
	result |= (result >>  1);
	result |= (result >>  2);
	result |= (result >>  4);
	result |= (result >>  8);
	result |= (result >> 16);
	
	return result + 1;
} 

// End fast trigonometric functions.

#endif
