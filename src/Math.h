#ifndef MATH_INCLUDED
#define MATH_INCLUDED

#include <math.h>
#include <stdlib.h>

//! Basic definitions

#define HALF_PI 0.5*M_PI

#define TRUE  1
#define FALSE 0

#ifndef MIN
  #define MIN(x,y) ((x)<(y)?(x):(y))
#endif

#ifndef MAX
  #define MAX(x,y) ((x)>(y)?(x):(y))
#endif

// Begin LIBGIMP

// LIBGIMP - The GIMP Library
// Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball

//! Clamp x at lower = l and upper = u
#ifndef CLAMP
  #define CLAMP(x,l,u) ((x)<(l)?(l):((x)>(u)?(u):(x)))
#endif

// /* Use RINT() instead of rint() */
// #ifdef HAVE_RINT
// #define RINT(x) rint(x)
// #else
// #define RINT(x) floor ((x) + 0.5)
// #endif

//! Round
#ifndef ROUND
  #define ROUND(x) ((int) ((x) + 0.5))
#endif

//! Square
#ifndef SQR
  #define SQR(x) ((x) * (x))
#endif

//! Limit a (0->511) int to 255
#ifndef MAX255
  #define MAX255(a)  ((a) | (((a) & 256) - (((a) & 256) >> 8)))
#endif

//! Clamp a int32-range int between 0 and 255 inclusive
#ifndef CLAMP0255
  #define CLAMP0255(a)  CLAMP(a,0,255)
#endif

// End LIBGIMP

// Fast array operations.

//! Adds #n# elements of #src1# to #src2# and puts the result in #dst#. 
// template <class T> void
// add(T *dst, const T *src1, const T *src2, size_t n)
// {
//   while (n--)
//     *dst++ = *src1++ + *src2++;
// }

//! Adds #n# elements of #src# to #dst#.
template <typename T, typename U> void
add(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ += static_cast<T>(*src++);
}

//! Adds #val# to #n# elements of #src#.
template <typename T, typename U> void
add(T *src, T val, size_t n)
{
  while (n--)
    *src++ += static_cast<T>(val);
}

// //! Subtracts #n# elements of #src2# from #src1# and puts the result in #dst#. 
// template <typename T, typename U> void
// subtract(T *dst, const T *src1, const T *src2, size_t n)
// {
//   while (n--)
//     *dst++ = *src1++ - *src2++;
// }

//! Subtracts #n# elements of #src# from #dst#.
template <typename T, typename U> void
subtract(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ -= static_cast<T>(*src++);
}

//! Subtracts #val# to #n# elements of #src#.
template <typename T, typename U> void
subtract(T *src, T val, size_t n)
{
  while (n--)
    *src++ -= static_cast<T>(val);
}

// //! Multiplies #n# elements of #src1# to #src2# and puts the result in #dst#. 
// template <typename T, typename U> void
// multiply(T *dst, const T *src1, const T *src2, size_t n)
// {
//   while (n--)
//     *dst++ = *src1++ * *src2++;
// }

//! Multiplies #n# elements of #dst# by #src#.
template <typename T, typename U> void
multiply(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ *= static_cast<T>(*src++);
}

//! Multiplies #val# with #n# elements of #src#.
template <typename T, typename U> void
multiply(T *src, U val, size_t n)
{
  while (n--)
    *src++ *= static_cast<T>(val);
}

// //! Divides #n# elements of #src2# from #src1# and puts the result in #dst#. 
// template <typename T, typename U> void
// divide(T *dst, const T *src1, const T *src2, size_t n)
// {
//   while (n--)
//     *dst++ = *src1++ / *src2++;
// }

//! Divides #n# elements of #dst# by #src#.
template <typename T, typename U> void
divide(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ /= static_cast<T>(*src++);
}

//! Divides #val# with #n# elements of #src#.
template <typename T, typename U> void
divide(T *src, U val, size_t n)
{
  while (n--)
    *src++ /= static_cast<T>(val);
}

//! Copies #n# values of #src# to #dst#. If both are of the same type, better use memcpy(3).
template <typename T, typename U> void
copy(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src++);
}

//! Returns the sum of #n# elements of #src#.
template <typename T> T
sum(const T *src, size_t n)
{
  if (!n)
    return 0;
  T ret = *src++;
  while (--n)
    ret += *src++;
  return ret;
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

#define FAST_MATH_PRECISION 0 // XXX to be changed ultimately (how??)

inline float fastsin (float x)
{
#if FAST_MATH_PRECISION
  float x2 = x*x;
  float ret = -2.39e-08f;
  ret *= x2;
  ret += 2.7526e-06f;
  ret *= x2;
  ret -= 1.98409e-04f;
  ret *= x2;
  ret += 8.3333315e-03f;
  ret *= x2;
  ret -= 1.666666664e-01f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#else
  float x2 = x*x;
  float ret = 7.61e-03f;
  ret *= x2;
  ret -= 1.6605e-01f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#endif
}

inline float fastcos (float x)
{
#if FAST_MATH_PRECISION
  float x2 = x*x;
  float ret = 3.705e-02f;
  ret *= x2;
  ret -= 4.967e-01f;
  ret *= x2;
  ret += 1.0f;
  return ret;
#else
  float x2 = x*x;
  float ret = -2.605e-07f;
  ret *= x2;
  ret += 2.47609e-05f;
  ret *= x2;
  ret -= 1.3888397e-03f;
  ret *= x2;
  ret += 4.16666418e-02f;
  ret *= x2;
  ret -= 4.999999963e-01f;
  ret *= x2;
  ret += 1.0f;
  return ret;
#endif
}

inline float fasttan (float x)
{
#if FAST_MATH_PRECISION
  float x2 = x*x;
  float ret = 2.033e-01f;
  ret *= x2;
  ret += 3.1755e-01f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#else
  float x2 = x*x;
  float ret = 9.5168091e-03f;
  ret *= x2;
  ret += 2.900525e-03f;
  ret *= x2;
  ret += 2.45650893e-02f;
  ret *= x2;
  ret += 5.33740603e-02f;
  ret *= x2;
  ret += 1.333923995e-01f;
  ret *= x2;
  ret += 3.333314036e-01f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#endif
}

inline float fastasin (float x)
{
  float xRoot = sqrt(1.0f-x);
  float ret = -0.0187293f;
  ret *= x;
  ret += 0.0742610f;
  ret *= x;
  ret -= 0.2121144f;
  ret *= x;
  ret += 1.5707288f;
  ret = HALF_PI - xRoot*ret;
  return ret;
}

inline float fastacos (float x)
{
  float xRoot = sqrt(1.0f-x);
  float ret = -0.0187293f;
  ret *= x;
  ret += 0.0742610f;
  ret *= x;
  ret -= 0.2121144f;
  ret *= x;
  ret += 1.5707288f;
  ret *= xRoot;
  return ret;
}

inline float fastatan (float x)
{
#if FAST_MATH_PRECISION
  float x2 = x*x;
  float ret = 0.0028662257f;
  ret *= x2;
  ret -= 0.0161657367f;
  ret *= x2;
  ret += 0.0429096138f;
  ret *= x2;
  ret -= 0.0752896400f;
  ret *= x2;
  ret += 0.1065626393f;
  ret *= x2;
  ret -= 0.1420889944f;
  ret *= x2;
  ret += 0.1999355085f;
  ret *= x2;
  ret -= 0.3333314528f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#else
  float x2 = x*x;
  float ret = 0.0208351f;
  ret *= x2;
  ret -= 0.085133f;
  ret *= x2;
  ret += 0.180141f;
  ret *= x2;
  ret -= 0.3302995f;
  ret *= x2;
  ret += 0.999866f;
  ret *= x;
  return ret;
#endif
}

//! End WML

#endif
