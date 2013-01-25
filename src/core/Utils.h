/* Utils.h
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <inttypes.h>
#include <string.h>

#include "ColorSpace.h"
#include "error.h"

// # RGB to grayscale operations ############################################

//! Transforms a RGB color to gray.
inline unsigned char rgb2gray(unsigned char r, unsigned char g, unsigned char b)
{
  return
    (unsigned char) ( ( 19595*(unsigned int)r +
                        38470*(unsigned int)g +
                        7471 *(unsigned int)b   ) >> 16 );
}

//! Fast version.
inline unsigned char fastrgb2gray(unsigned char r, unsigned char g, unsigned char b)
{
  return (unsigned char) ( ((unsigned int)(r + (g<<1) + b)) >> 2 );
}

//! Vector version. Sets #dst# to the grayscale of #src#.
inline void grayscaleRGBA(RGBA *dst, const RGBA *src, size_t size)
{
  ASSERT_ERROR(dst != NULL);
  ASSERT_ERROR(src != NULL);
  unsigned int gray;
  unsigned int *it = (unsigned int *)dst;
  while (size--)
  {
    gray = rgb2gray(src->r, src->g, src->b);
    *it++ = gray | gray<<8 | gray<<16;
    src++;
  }
}

//! Puts in #dst# the grayscale values of #src#.
inline void grayscaleChannel(unsigned char *dst, const RGBA *src, size_t size)
{
  ASSERT_ERROR(dst != NULL);
  ASSERT_ERROR(src != NULL);
  while (size--)
  {
    *dst++ = rgb2gray(src->r, src->g, src->b);
    src++;
  }
}

//! Puts the #channelIdx#-th channel of #src# (i.e. one of the IDX_RGBA_{R,G,B,A} constants) in #dst#.
inline void extractChannel(unsigned char *dst, const RGBA *src, size_t size, int channelIdx)
{
  ASSERT_ERROR(dst != NULL);
  ASSERT_ERROR(src != NULL);
  ASSERT_ERROR(0 <= channelIdx && channelIdx < SIZE_RGBA);
  unsigned char *it = (unsigned char*)(src) + channelIdx; // offset
  while (size--)
  {
    *dst++ = *it;
    it += SIZE_RGBA;
  }
}

//! Fast converts 24-bits color to 32 bits (alpha is set to specified alpha value).
// Based on: http://stackoverflow.com/questions/7069090/convert-rgb-to-rgba-in-c
inline void convert24to32(unsigned char *dst, const unsigned char *src, size_t size, unsigned char alpha=0xff) {
  ASSERT_ERROR(dst != NULL);
  ASSERT_ERROR(src != NULL);
  if (size==0) return;
  uint32_t alphaMask = ((uint32_t)alpha) << 24;
  // Copy by 4 byte blocks.
  for (size_t i=size; --i; dst+=4, src+=3)
  {
    *(uint32_t*)(void*)dst = (*(const uint32_t*)(const void*)src) | alphaMask;
  }
  // Copy remaining bytes.
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
}

inline void rgb2rgba(RGBA *dst, const RGB *src, size_t size, unsigned char alpha=0xff) {
  convert24to32((unsigned char*)dst, (const unsigned char*)src, size, alpha);
}

// # Image rescaling function ###############################################

inline void rescale_image(RGBA *dst, const RGBA *src, int dstWidth, int dstHeight, int srcWidth, int srcHeight)
{
  NOTICE("%d %d %d %d", dstWidth, dstHeight, srcWidth, srcHeight);
  if (dstWidth == srcWidth && dstHeight == srcHeight)
  {
    // Same dimensions, just copy the data.
    memcpy(dst, src, dstHeight*dstWidth*sizeof(RGBA));
    return;
  }
  long xInc, xCur, yInc, yCur;
  xInc = (long)((long)srcWidth*65535L)/(long)dstWidth;
  yInc = (long)((long)srcHeight*65535L)/(long)dstHeight;
  register int i,j;
  const RGBA *srcIter = src;
  yCur = 0;
  for (j=0; j<dstHeight; ++j)
  {
    xCur = 0;
    for (i=0; i<dstWidth; ++i)
    {
      dst[i] = srcIter[xCur>>16];
      xCur+=xInc;
    }
    srcIter = &src[(yCur>>16) * srcWidth];
    dst += dstWidth;
    yCur+=yInc;
  }
}

// # Basic paint functions (from the Gimp paint-funcs-generic.h) ############

/* Opacities */
#define TRANSPARENT_OPACITY        0
#define OPAQUE_OPACITY             255

#define INT_MULT(a,b,t)  ((t) = (a) * (b) + 0x80, ((((t) >> 8) + (t)) >> 8))

/* This version of INT_MULT3 is very fast, but suffers from some
   slight roundoff errors.  It returns the correct result 99.987
   percent of the time */
#define INT_MULT3(a,b,c,t)  ((t) = (a) * (b) * (c) + 0x7F5B, \
                            ((((t) >> 7) + (t)) >> 16))
/*
  This version of INT_MULT3 always gives the correct result, but runs at
  approximatly one third the speed. */
/*  #define INT_MULT3(a,b,c,t) (((a) * (b) * (c) + 32512) / 65025.0)
 */

#define INT_BLEND(a,b,alpha,tmp)  (INT_MULT((a) - (b), alpha, tmp) + (b))

#define RANDOM_TABLE_SIZE  4096
#define RANDOM_SEED 43343124

/* A drawable has an alphachannel if contains either 4 or 2 bytes data
 * aka GRAYA and RGBA and thus the macro below works. This will have
 * to change if we support bigger formats. We'll do it so for now because
 * masking is always cheaper than passing parameters over the stack.      */
/* FIXME: Move to a global place */

#ifndef HAS_ALPHA
#define HAS_ALPHA(bytes) (~bytes & 1)
//#define HAS_ALPHA(bytes) 0 // XXX pour le moment on l'utilise pas, donc...
#endif

static unsigned char       add_lut[511];
static int                 random_table[RANDOM_TABLE_SIZE];

inline void
paint_funcs_setup (void)
{
  int i;

  /*  generate a table of random seeds  */
  srand(RANDOM_SEED);

  for (i = 0; i < RANDOM_TABLE_SIZE; i++)
    random_table[i] = rand();

  for (i = 0; i < 256; i++)
    add_lut[i] = i;

  for (i = 256; i <= 510; i++)
    add_lut[i] = 255;
}

inline void
paint_funcs_free (void)
{
}

inline void
blend_pixels (const unsigned char *src1,
              const unsigned char *src2,
              unsigned char       *dest,
              unsigned char        blend,
              unsigned int         w,
              unsigned int         bytes)
{
  unsigned int b;

  if (HAS_ALPHA (bytes))
  {
    const unsigned int blend1 = 256 - blend;
    const unsigned int blend2 = blend + 1;
    const unsigned int c      = bytes - 1;

    while (w--)
    {
      unsigned int a1 = blend1 * src1[c];
      unsigned int a2 = blend2 * src2[c];
      unsigned int a  = a1 + a2;

      if (!a)
      {
        for (b = 0; b < bytes; b++)
          dest[b] = 0;
      } else
      {
        for (b = 0; b < c; b++)
          dest[b] = (src1[b] * a1 + src2[b] * a2) / a;

        dest[c] = a >> 8;
      }

      src1 += bytes;
      src2 += bytes;
      dest += bytes;
    }
  } else
  {
    const unsigned char blend1 = 255 - blend;

    while (w--)
    {
      for (b = 0; b < bytes; b++)
        dest[b] = (src1[b] * blend1 + src2[b] * blend) / 255;

      src1 += bytes;
      src2 += bytes;
      dest += bytes;
    }
  }
}

inline void
shade_pixels (const unsigned char *src,
              const unsigned char *col,
              unsigned char       *dest,
              unsigned char     blend,
              unsigned int         w,
              unsigned int         bytes,
              unsigned int         has_alpha)
{
  const unsigned char blend2 = (255 - blend);
  const unsigned int  alpha = (has_alpha) ? bytes - 1 : bytes;
  unsigned int b;

  while (w--)
  {
    for (b = 0; b < alpha; b++)
      dest[b] = (src[b] * blend2 + col[b] * blend) / 255;

    if (has_alpha)
      dest[alpha] = src[alpha];  /* alpha channel */

    src += bytes;
    dest += bytes;
  }
}


inline void
extract_alpha_pixels (const unsigned char *src,
                      const unsigned char *mask,
                      unsigned char       *dest,
                      unsigned int         w,
                      unsigned int         bytes)
{
  const unsigned int alpha = bytes - 1;
  int          tmp;

  if (mask)
  {
    const unsigned char *m = mask;
    while (w--)
    {
      *dest++ = INT_MULT(src[alpha], *m, tmp);
      m++;
      src += bytes;
    }
  } else
  {
    while (w--)
    {
      *dest++ = INT_MULT(src[alpha], OPAQUE_OPACITY, tmp);
      src += bytes;
    }
  }
}

inline void
darken_pixels (const unsigned char *src1,
               const unsigned char *src2,
               unsigned char       *dest,
               unsigned int         length,
               unsigned int         bytes1,
               unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b;
  unsigned char s1, s2;

  while (length--)
  {
    for (b = 0; b < alpha; b++)
    {
      s1 = src1[b];
      s2 = src2[b];
      dest[b] = (s1 < s2) ? s1 : s2;
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}

inline void
lighten_pixels (const unsigned char *src1,
                const unsigned char *src2,
                unsigned char       *dest,
                unsigned int         length,
                unsigned int         bytes1,
                unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b;
  unsigned char s1, s2;

  while (length--)
  {
    for (b = 0; b < alpha; b++)
    {
      s1 = src1[b];
      s2 = src2[b];
      dest[b] = (s1 < s2) ? s2 : s1;
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
hue_only_pixels (const unsigned char *src1,
                 const unsigned char *src2,
                 unsigned char       *dest,
                 unsigned int         length,
                 unsigned int         bytes1,
                 unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  int r1, g1, b1;
  int r2, g2, b2;

  /*  assumes inputs are only 4 byte RGBA pixels  */
  while (length--)
  {
    r1 = src1[0]; g1 = src1[1]; b1 = src1[2];
    r2 = src2[0]; g2 = src2[1]; b2 = src2[2];
    rgb_to_hsv_int (&r1, &g1, &b1);
    rgb_to_hsv_int (&r2, &g2, &b2);

    r1 = r2;

    /*  set the destination  */
    hsv_to_rgb_int (&r1, &g1, &b1);

    dest[0] = r1; dest[1] = g1; dest[2] = b1;

    if (has_alpha1 && has_alpha2)
      dest[3] = MIN (src1[3], src2[3]);
    else if (has_alpha2)
      dest[3] = src2[3];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
saturation_only_pixels (const unsigned char *src1,
                        const unsigned char *src2,
                        unsigned char       *dest,
                        unsigned int         length,
                        unsigned int         bytes1,
                        unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  int r1, g1, b1;
  int r2, g2, b2;

  /*  assumes inputs are only 4 byte RGBA pixels  */
  while (length--)
  {
    r1 = src1[0]; g1 = src1[1]; b1 = src1[2];
    r2 = src2[0]; g2 = src2[1]; b2 = src2[2];
    rgb_to_hsv_int (&r1, &g1, &b1);
    rgb_to_hsv_int (&r2, &g2, &b2);

    g1 = g2;

    /*  set the destination  */
    hsv_to_rgb_int (&r1, &g1, &b1);

    dest[0] = r1; dest[1] = g1; dest[2] = b1;

    if (has_alpha1 && has_alpha2)
      dest[3] = MIN (src1[3], src2[3]);
    else if (has_alpha2)
      dest[3] = src2[3];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
value_only_pixels (const unsigned char *src1,
                   const unsigned char *src2,
                   unsigned char       *dest,
                   unsigned int         length,
                   unsigned int         bytes1,
                   unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  int r1, g1, b1;
  int r2, g2, b2;

  /*  assumes inputs are only 4 byte RGBA pixels  */
  while (length--)
  {
    r1 = src1[0]; g1 = src1[1]; b1 = src1[2];
    r2 = src2[0]; g2 = src2[1]; b2 = src2[2];
    rgb_to_hsv_int (&r1, &g1, &b1);
    rgb_to_hsv_int (&r2, &g2, &b2);

    b1 = b2;

    /*  set the destination  */
    hsv_to_rgb_int (&r1, &g1, &b1);

    dest[0] = r1; dest[1] = g1; dest[2] = b1;

    if (has_alpha1 && has_alpha2)
      dest[3] = MIN (src1[3], src2[3]);
    else if (has_alpha2)
      dest[3] = src2[3];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
color_only_pixels (const unsigned char *src1,
                   const unsigned char *src2,
                   unsigned char       *dest,
                   unsigned int         length,
                   unsigned int         bytes1,
                   unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  unsigned int r1, g1, b1;
  unsigned int r2, g2, b2;

  /*  assumes inputs are only 4 byte RGBA pixels  */
  while (length--)
  {
    r1 = src1[0]; g1 = src1[1]; b1 = src1[2];
    r2 = src2[0]; g2 = src2[1]; b2 = src2[2];
    rgb_to_hsl_int (&r1, &g1, &b1);
    rgb_to_hsl_int (&r2, &g2, &b2);

    /*  transfer hue and saturation to the source pixel  */
    r1 = r2;
    g1 = g2;

    /*  set the destination  */
    hsl_to_rgb_int (&r1, &g1, &b1);

    dest[0] = r1; dest[1] = g1; dest[2] = b1;

    if (has_alpha1 && has_alpha2)
      dest[3] = MIN (src1[3], src2[3]);
    else if (has_alpha2)
      dest[3] = src2[3];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
multiply_pixels (const unsigned char *src1,
                 const unsigned char *src2,
                 unsigned char       *dest,
                 unsigned int         length,
                 unsigned int         bytes1,
                 unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b, tmp;

  if (has_alpha1 && has_alpha2)
  {
    while (length --)
    {
      for (b = 0; b < alpha; b++)
        dest[b] = INT_MULT(src1[b], src2[b], tmp);

      dest[alpha] = MIN (src1[alpha], src2[alpha]);

      src1 += bytes1;
      src2 += bytes2;
      dest += bytes2;
    }
  } else if (has_alpha2)
  {
    while (length --)
    {
      for (b = 0; b < alpha; b++)
        dest[b] = INT_MULT(src1[b], src2[b], tmp);

      dest[alpha] = src2[alpha];

      src1 += bytes1;
      src2 += bytes2;
      dest += bytes2;
    }
  } else
  {
    while (length --)
    {
      for (b = 0; b < alpha; b++)
        dest[b] = INT_MULT(src1[b], src2[b], tmp);

      src1 += bytes1;
      src2 += bytes2;
      dest += bytes2;
    }
  }
}


inline void
divide_pixels (const unsigned char *src1,
               const unsigned char *src2,
               unsigned char       *dest,
               unsigned int         length,
               unsigned int         bytes1,
               unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b, result;

  while (length--)
  {
    for (b = 0; b < alpha; b++)
    {
      result = ((src1[b] << 8) / (1+src2[b]));
      //result = ((src1[b] * 256) / (1+src2[b]));
      dest[b] = MIN (result, 255u);
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
screen_pixels (const unsigned char *src1,
               const unsigned char *src2,
               unsigned char       *dest,
               unsigned int         length,
               unsigned int         bytes1,
               unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b, tmp;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
      dest[b] = 255 - INT_MULT((255 - src1[b]), (255 - src2[b]), tmp);

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
overlay_pixels (const unsigned char *src1,
                const unsigned char *src2,
                unsigned char       *dest,
                unsigned int         length,
                unsigned int         bytes1,
                unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b, tmp, tmpM;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
    {
      dest[b] = INT_MULT(src1[b], src1[b] + INT_MULT(2 * src2[b],
                                                     255 - src1[b],
                                                     tmpM), tmp);
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}

inline void
dodge_pixels (const unsigned char *src1,
              const unsigned char *src2,
              unsigned char    *dest,
              unsigned int         length,
              unsigned int         bytes1,
              unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b, tmp;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
    {
      tmp = src1[b] << 8;
      tmp /= 256 - src2[b];
      dest[b] = (unsigned char) MIN (tmp, 255u);
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
burn_pixels (const unsigned char *src1,
             const unsigned char *src2,
             unsigned char       *dest,
             unsigned int         length,
             unsigned int         bytes1,
             unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b;

  /* FIXME: Is the burn effect supposed to be dependant on the sign of this
   * temporary variable? */
  int tmp;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
    {
      tmp = (255 - src1[b]) << 8;
      tmp /= src2[b] + 1;
      dest[b] = (unsigned char) CLAMP (255 - tmp, 0, 255);
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


static inline void
hardlight_pixels (const unsigned char *src1,
                  const unsigned char *src2,
                  unsigned char       *dest,
                  unsigned int         length,
                  unsigned int         bytes1,
                  unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b, tmp;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
    {
      if (src2[b] > 128)
      {
        tmp = ((int)255 - src1[b]) * ((int)255 - ((src2[b] - 128) << 1));
        dest[b] = (unsigned char) MIN (255 - (tmp >> 8), 255u);
      } else
      {
        tmp = (int)src1[b] * ((int)src2[b] << 1);
        dest[b] = (unsigned char) MIN (tmp >> 8, 255u);
      }
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
softlight_pixels (const unsigned char *src1,
                  const unsigned char *src2,
                  unsigned char       *dest,
                  unsigned int         length,
                  unsigned int         bytes1,
                  unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b, tmpS, tmpM, tmp1, tmp2, tmp3;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
    {
      /* Mix multiply and screen */
      tmpM = INT_MULT (src1[b], src2[b], tmpM);
      tmpS = 255 - INT_MULT((255 - src1[b]), (255 - src2[b]), tmp1);
      dest[b] = INT_MULT ((255 - src1[b]), tmpM, tmp2) +
                INT_MULT (src1[b], tmpS, tmp3);
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
grain_extract_pixels (const unsigned char *src1,
                      const unsigned char *src2,
                      unsigned char       *dest,
                      unsigned int         length,
                      unsigned int         bytes1,
                      unsigned int         bytes2)
{
  unsigned int alpha, b;
  int diff;
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);

  alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
    {
      diff = src1[b] - src2[b] + 128;
      dest[b] = (unsigned char) CLAMP (diff, 0, 255);
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
grain_merge_pixels (const unsigned char *src1,
                    const unsigned char *src2,
                    unsigned char       *dest,
                    unsigned int         length,
                    unsigned int         bytes1,
                    unsigned int         bytes2)
{
  int alpha, b;
  int sum;
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);

  alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
    {
      /* Add, re-center and clip. */
      sum = src1[b] + src2[b] - 128;
      dest[b] = (unsigned char) CLAMP (sum, 0, 255);
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
add_pixels (const unsigned char *src1,
            const unsigned char *src2,
            unsigned char       *dest,
            unsigned int         length,
            unsigned int         bytes1,
            unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
      dest[b] = add_lut[src1[b] + src2[b]];

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
subtract_pixels (const unsigned char *src1,
                 const unsigned char *src2,
                 unsigned char       *dest,
                 unsigned int         length,
                 unsigned int         bytes1,
                 unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b;
  int diff;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
    {
      diff = src1[b] - src2[b];
      dest[b] = (diff < 0) ? 0 : diff;
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


static inline void
difference_pixels (const unsigned char *src1,
                   const unsigned char *src2,
                   unsigned char       *dest,
                   unsigned int         length,
                   unsigned int         bytes1,
                   unsigned int         bytes2)
{
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  unsigned int b;
  int diff;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
    {
      diff = src1[b] - src2[b];
      dest[b] = (diff < 0) ? -diff : diff;
    }

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}


inline void
dissolve_pixels (const unsigned char *src,
                 unsigned char       *mask,
                 unsigned char       *dest,
                 int          x,
                 int          y,
                 int          opacity,
                 int          length,
                 int          sb,
                 int          db,
                 unsigned int        has_alpha)
{
  int    alpha, b;
  int  rand_val;
  int    combined_opacity;

  srand(random_table[y % RANDOM_TABLE_SIZE]);

  /* Ignore x random values so we get a deterministic result */
  for (b = 0; b < x; b ++)
    rand();

  alpha = db - 1;

  while (length--)
  {
    /*  preserve the intensity values  */
    for (b = 0; b < alpha; b++)
      dest[b] = src[b];

    /*  dissolve if random value is >= opacity  */
    rand_val = rand() % 256;

    if (mask)
    {
      if (has_alpha)
        combined_opacity = opacity * src[alpha] * *mask / (255 * 255);
      else
        combined_opacity = opacity * *mask / 255;

      mask++;
    } else
    {
      if (has_alpha)
        combined_opacity = opacity * src[alpha] / 255;
      else
        combined_opacity = opacity;
    }

    dest[alpha] = (rand_val >= combined_opacity) ? 0 : OPAQUE_OPACITY;

    src  += sb;
    dest += db;
  }

}

inline void
replace_pixels (unsigned char   *src1,
                unsigned char   *src2,
                unsigned char   *dest,
                unsigned char   *mask,
                int      length,
                int      opacity,
                bool *affect,
                int      bytes1,
                int      bytes2)
{
  int    alpha;
  int    b;
  double a_val, a_recip, mask_val;
  double norm_opacity;
  int    s1_a, s2_a;
  int    new_val;

  if (bytes1 != bytes2)
  {
    std::cerr << "replace_pixels only works on commensurate pixel regions" << std::endl;
    return;
  }

  alpha = bytes1 - 1;
  norm_opacity = opacity * (1.0 / 65536.0);

  while (length --)
  {
    mask_val = mask[0] * norm_opacity;
    /* calculate new alpha first. */
    s1_a = src1[alpha];
    s2_a = src2[alpha];
    a_val = s1_a + mask_val * (s2_a - s1_a);

    if (a_val == 0) /* In any case, write out versions of the blending function */
    /* that result when combinations of s1_a, s2_a, and         */
    /* mask_val --> 0 (or mask_val -->1)                        */
    {
      /* Case 1: s1_a, s2_a, AND mask_val all approach 0+:               */
      /* Case 2: s1_a AND s2_a both approach 0+, regardless of mask_val: */

      if (s1_a + s2_a == 0.0)
      {
        for (b = 0; b < alpha; b++)
        {
          new_val = (int) ( 0.5 + (double) src1[b] +
                            mask_val * ((double) src2[b] - (double) src1[b]) );

          dest[b] = affect[b] ? MIN (new_val, 255) : src1[b];
        }
      }

      /* Case 3: mask_val AND s1_a both approach 0+, regardless of s2_a  */
      else if (s1_a + mask_val == 0.0)
      {
        for (b = 0; b < alpha; b++)
        {
          dest[b] = src1[b];
        }
      }

      /* Case 4: mask_val -->1 AND s2_a -->0, regardless of s1_a         */
      else if (1.0 - mask_val + s2_a == 0.0)
      {
        for (b = 0; b < alpha; b++)
        {
          dest[b] = affect[b] ? src2[b] : src1[b];
        }
      }
    } else
    {
      a_recip = 1.0 / a_val;
      /* possible optimization: fold a_recip into s1_a and s2_a              */
      for (b = 0; b < alpha; b++)
      {
        new_val = (int) ( 0.5 + a_recip * (src1[b] * s1_a + mask_val *
                                           (src2[b] * s2_a - src1[b] * s1_a)) );
        dest[b] = affect[b] ? MIN (new_val, 255) : src1[b];
      }
    }

    dest[alpha] = (unsigned char) (affect[alpha] ? a_val + 0.5: s1_a);
    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
    mask++;
  }
}

inline void
swap_pixels (unsigned char *src,
             unsigned char *dest,
             unsigned int   length)
{
  while (length--)
  {
    *src = *src ^ *dest;
    *dest = *dest ^ *src;
    *src = *src ^ *dest;
    src++;
    dest++;
  }
}

inline void
scale_pixels (const unsigned char *src,
              unsigned char       *dest,
              unsigned int         length,
              int          scale)
{
  int tmp;

  while (length --)
  {
    *dest++ = (unsigned char) INT_MULT (*src, scale, tmp);
    src++;
  }
}

inline void
add_alpha_pixels (const unsigned char *src,
                  unsigned char       *dest,
                  unsigned int         length,
                  unsigned int         bytes)
{
  unsigned int alpha, b;

  alpha = bytes + 1;

  while (length --)
  {
    for (b = 0; b < bytes; b++)
      dest[b] = src[b];

    dest[b] = OPAQUE_OPACITY;

    src += bytes;
    dest += alpha;
  }
}

inline void
flatten_pixels (const unsigned char *src,
                const unsigned char *bg,
                unsigned char       *dest,
                unsigned int         length,
                unsigned int         bytes)
{
  int alpha, b;
  int t1, t2;

  alpha = bytes - 1;
  while (length --)
  {
    for (b = 0; b < alpha; b++)
      dest[b] = INT_MULT (src[b], src[alpha], t1) +
                INT_MULT (bg[b], (255 - src[alpha]), t2);

    src += bytes;
    dest += alpha;
  }
}

inline void
gray_to_rgb_pixels (const unsigned char *src,
                    unsigned char       *dest,
                    unsigned int         length,
                    unsigned int         bytes)
{
  unsigned int     b;
  int     dest_bytes;
  bool has_alpha;

  has_alpha = (bytes == 2) ? TRUE : FALSE;
  dest_bytes = (has_alpha) ? 4 : 3;

  while (length --)
  {
    for (b = 0; b < bytes; b++)
      dest[b] = src[0];

    if (has_alpha)
      dest[3] = src[1];

    src += bytes;
    dest += dest_bytes;
  }
}

inline void
apply_mask_to_alpha_channel (unsigned char       *src,
                             const unsigned char *mask,
                             unsigned int         opacity,
                             unsigned int         length,
                             unsigned int         bytes)
{
  long tmp;

  src += bytes - 1;

  if (opacity == 255)
  {
    while (length --)
    {
      *src = INT_MULT(*src, *mask, tmp);
      mask++;
      src += bytes;
    }
  } else
  {
    while (length --)
    {
      *src = INT_MULT3(*src, *mask, opacity, tmp);
      mask++;
      src += bytes;
    }
  }
}

inline void
combine_mask_and_alpha_channel (unsigned char       *src,
                                const unsigned char *mask,
                                unsigned int         opacity,
                                unsigned int         length,
                                unsigned int         bytes)
{
  int mask_val;
  int alpha;
  int tmp;

  alpha = bytes - 1;
  src += alpha;

  if (opacity != 255)
    while (length --)
    {
      mask_val = INT_MULT(*mask, opacity, tmp);
      mask++;
      *src = *src + INT_MULT((255 - *src) , mask_val, tmp);
      src += bytes;
    }
  else
    while (length --)
    {
      *src = *src + INT_MULT((255 - *src) , *mask, tmp);
      src += bytes;
      mask++;
    }
}


inline void
copy_gray_to_inten_a_pixels (const unsigned char *src,
                             unsigned char       *dest,
                             unsigned int         length,
                             unsigned int         bytes)
{
  int b;
  int alpha;

  alpha = bytes - 1;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
      dest[b] = *src;
    dest[b] = OPAQUE_OPACITY;

    src ++;
    dest += bytes;
  }
}


inline void
initial_channel_pixels (const unsigned char *src,
                        unsigned char       *dest,
                        unsigned int         length,
                        unsigned int         bytes)
{
  int alpha, b;

  alpha = bytes - 1;

  while (length --)
  {
    for (b = 0; b < alpha; b++)
      dest[b] = src[0];

    dest[alpha] = OPAQUE_OPACITY;

    dest += bytes;
    src ++;
  }
}


inline void
initial_indexed_pixels (const unsigned char *src,
                        unsigned char       *dest,
                        const unsigned char *cmap,
                        unsigned int         length)
{
  int col_index;

  /*  This function assumes always that we're mapping from
   *  an RGB colormap to an RGBA image...
   */
  while (length--)
  {
    col_index = *src++ * 3;
    *dest++ = cmap[col_index++];
    *dest++ = cmap[col_index++];
    *dest++ = cmap[col_index++];
    *dest++ = OPAQUE_OPACITY;
  }
}


inline void
initial_indexed_a_pixels (const unsigned char *src,
                          unsigned char       *dest,
                          const unsigned char *mask,
                          const unsigned char *no_mask,
                          const unsigned char *cmap,
                          unsigned int         opacity,
                          unsigned int         length)
{
  int          col_index;
  unsigned char        new_alpha;
  const unsigned char *m;
  long         tmp;

  if (mask)
    m = mask;
  else
    m = no_mask;

  while (length --)
  {
    col_index = *src++ * 3;
    new_alpha = INT_MULT3(*src, *m, opacity, tmp);
    src++;
    *dest++ = cmap[col_index++];
    *dest++ = cmap[col_index++];
    *dest++ = cmap[col_index++];
    /*  Set the alpha channel  */
    *dest++ = (new_alpha > 127) ? OPAQUE_OPACITY : TRANSPARENT_OPACITY;

    if (mask)
      m++;
  }
}


inline void
initial_inten_pixels (const unsigned char *src,
                      unsigned char       *dest,
                      const unsigned char *mask,
                      const unsigned char *no_mask,
                      unsigned int         opacity,
                      const int   *affect,
                      unsigned int         length,
                      unsigned int         bytes)
{
  unsigned int  b;
  int  tmp;
  int  l;
  const unsigned char *m;
  unsigned char       *destp;
  const unsigned char *srcp;
  const int    dest_bytes = bytes + 1;

  if (mask)
  {
    m = mask;

    /*  This function assumes the source has no alpha channel and
     *  the destination has an alpha channel.  So dest_bytes = bytes + 1
     */

    if (bytes == 3 && affect[0] && affect[1] && affect[2])
    {
      if (!affect[bytes])
        opacity = 0;

      destp = dest + bytes;

      if (opacity != 0)
        while (length--)
        {
          dest[0] = src[0];
          dest[1] = src[1];
          dest[2] = src[2];
          dest[3] = INT_MULT(opacity, *m, tmp);
          src  += bytes;
          dest += dest_bytes;
          m++;
        }
      else
        while (length--)
        {
          dest[0] = src[0];
          dest[1] = src[1];
          dest[2] = src[2];
          dest[3] = opacity;
          src  += bytes;
          dest += dest_bytes;
        }
      return;
    }

    for (b =0; b < bytes; b++)
    {
      destp = dest + b;
      srcp = src + b;
      l = length;

      if (affect[b])
        while (l--)
        {
          *destp = *srcp;
          srcp  += bytes;
          destp += dest_bytes;
        }
      else
        while (l--)
        {
          *destp = 0;
          destp += dest_bytes;
        }
    }

    /* fill the alpha channel */
    if (!affect[bytes])
      opacity = 0;

    destp = dest + bytes;

    if (opacity != 0)
      while (length--)
      {
        *destp = INT_MULT(opacity , *m, tmp);
        destp += dest_bytes;
        m++;
      }
    else
      while (length--)
      {
        *destp = opacity;
        destp += dest_bytes;
      }
  }

  /* If no mask */
  else
  {
    m = no_mask;

    /*  This function assumes the source has no alpha channel and
     *  the destination has an alpha channel.  So dest_bytes = bytes + 1
     */

    if (bytes == 3 && affect[0] && affect[1] && affect[2])
    {
      if (!affect[bytes])
        opacity = 0;

      destp = dest + bytes;

      while (length--)
      {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
        dest[3] = opacity;
        src  += bytes;
        dest += dest_bytes;
      }
      return;
    }

    for (b =0; b < bytes; b++)
    {
      destp = dest + b;
      srcp = src + b;
      l = length;

      if (affect[b])
        while (l--)
        {
          *destp = *srcp;
          srcp  += bytes;
          destp += dest_bytes;
        }
      else
        while (l--)
        {
          *destp = 0;
          destp += dest_bytes;
        }
    }

    /* fill the alpha channel */
    if (!affect[bytes])
      opacity = 0;

    destp = dest + bytes;

    while (length--)
    {
      *destp = opacity;
      destp += dest_bytes;
    }
  }
}


inline void
initial_inten_a_pixels (const unsigned char   *src,
                        unsigned char         *dest,
                        const unsigned char   *mask,
                        unsigned int           opacity,
                        const bool *affect,
                        unsigned int           length,
                        unsigned int           bytes)
{
  int          alpha, b;
  const unsigned char *m;
  long         tmp;

  alpha = bytes - 1;
  if (mask)
  {
    m = mask;
    while (length --)
    {
      for (b = 0; b < alpha; b++)
        dest[b] = src[b] * affect[b];

      /*  Set the alpha channel  */
      dest[alpha] = affect [alpha] ? INT_MULT3(opacity, src[alpha], *m, tmp)
                    : 0;

      m++;

      dest += bytes;
      src += bytes;
    }
  } else
  {
    while (length --)
    {
      for (b = 0; b < alpha; b++)
        dest[b] = src[b] * affect[b];

      /*  Set the alpha channel  */
      dest[alpha] = affect [alpha] ? INT_MULT(opacity , src[alpha], tmp) : 0;

      dest += bytes;
      src += bytes;
    }
  }
}

inline void
component_pixels (const unsigned char *src,
                  unsigned char       *dest,
                  unsigned int         length,
                  unsigned int         bytes,
                  unsigned int         pixel)
{
  src += pixel;

  while (length --)
  {
    *dest = *src;

    src += bytes;
    dest++;
  }
}

// ##########################################################################

// # Threshold operation ####################################################

inline void
threshold_pixels (const unsigned char *src,
                  unsigned char       *dest,
                  unsigned char        threshold,
                  unsigned int         length,
                  unsigned int         bytes)
{
  unsigned int b;
  const unsigned int alpha = HAS_ALPHA (bytes) ? bytes - 1 : bytes;
  
  if (HAS_ALPHA (bytes))
  {
    while (length --)
    {
      for (b = 0; b < alpha; b++)
        dest[b] = (src[b] > threshold ? 255 : 0);
      
      dest[alpha] = src[alpha];
      dest += bytes;
      src += bytes;
    }
  }
  else
  {
    while (length --)
    {
      for (b = 0; b < alpha; b++)
        dest[b] = (src[b] > threshold ? 255 : 0);
      
      dest += bytes;
      src += bytes;
    }
  }
}

// # Logical operators ######################################################

inline void
invert_pixels (const unsigned char *src,
               unsigned char       *dest,
               unsigned int         length,
               unsigned int         bytes)
{
  unsigned int b;
  const unsigned int alpha = HAS_ALPHA (bytes) ? bytes - 1 : bytes;
  
  if (HAS_ALPHA (bytes))
  {
    while (length --)
    {
      for (b = 0; b < alpha; b++)
        dest[b] = 0xff ^ src[b];
      
      dest[alpha] = src[alpha];
      dest += bytes;
      src += bytes;
    }
  }
  else
  {
    while (length --)
    {
      for (b = 0; b < alpha; b++)
        dest[b] = 0xff ^ src[b];
      
      dest += bytes;
      src += bytes;
    }
  }
}

inline void
and_pixels (const unsigned char *src1,
            const unsigned char *src2,
            unsigned char       *dest,
            unsigned int         length,
            unsigned int         bytes1,
            unsigned int         bytes2)
  
{
  unsigned int b;
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  
  while (length --)
  {
    for (b = 0; b < alpha; b++)
      dest[b] = src1[b] & src2[b];

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}

inline void
or_pixels (const unsigned char *src1,
           const unsigned char *src2,
           unsigned char       *dest,
           unsigned int         length,
           unsigned int         bytes1,
           unsigned int         bytes2)
{
  unsigned int b;
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  
  while (length --)
  {
    for (b = 0; b < alpha; b++)
      dest[b] = src1[b] | src2[b];

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}

inline void
xor_pixels (const unsigned char *src1,
            const unsigned char *src2,
            unsigned char       *dest,
            unsigned int         length,
            unsigned int         bytes1,
            unsigned int         bytes2)
{
  unsigned int b;
  const unsigned int has_alpha1 = HAS_ALPHA (bytes1);
  const unsigned int has_alpha2 = HAS_ALPHA (bytes2);
  const unsigned int alpha = (has_alpha1 || has_alpha2) ? MAX (bytes1, bytes2) - 1 : bytes1;
  
  while (length --)
  {
    for (b = 0; b < alpha; b++)
      dest[b] = src1[b] ^ src2[b];

    if (has_alpha1 && has_alpha2)
      dest[alpha] = MIN (src1[alpha], src2[alpha]);
    else if (has_alpha2)
      dest[alpha] = src2[alpha];

    src1 += bytes1;
    src2 += bytes2;
    dest += bytes2;
  }
}

#endif
