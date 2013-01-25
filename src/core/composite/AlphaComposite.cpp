/* AlphaComposite.cpp
 * Copyright (C) 2005 Jean-Sebastien Senecal
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

#include "AlphaComposite.h"
#include "Utils.h"

void alpha_premultiply(unsigned char *src,
                       unsigned int n)
{
  unsigned int tmp;
  while (n--)
  {
    for (int i=0; i<SIZE_RGB; ++i)
      src[i] = INT_MULT(src[i], src[IDX_RGBA_A], tmp);
    src += SIZE_RGBA;
  }
}

void alpha_demultiply(unsigned char *src,
                      unsigned int n)
{
  float mult;
  while (n--)
  {
    mult = (src[IDX_RGBA_A] == 0 ? 255.0f : 255.0f / (float)src[IDX_RGBA_A]);
    for (int i=0; i<SIZE_RGB; ++i)
      src[i] = CLAMP0255( (unsigned int) ((float)src[i] * mult ) );
    src += SIZE_RGBA;
  }
}

// XXX faut un argument "bool recompute_colors"

void alpha_set(unsigned char *src,
               const unsigned char *mask,
               unsigned int n)
{
  src += SIZE_RGB;
  while (n--)
  {
    *src = *mask++;
    src += SIZE_RGBA;
  }
}

void alpha_fill(unsigned char *src,
                unsigned char alpha,
                unsigned int n)
{
  src += SIZE_RGB;
  while (n--)
  {
    *src = alpha;
    src += SIZE_RGBA;
  }
}

void alpha_invert(unsigned char *src,
                  unsigned int n)                  
{
  //unsigned int b;
  unsigned  alpha;
  while (n--)
  {
    alpha = (255 - src[IDX_RGBA_A]);
    multiplyVecVal(src, src, (float)alpha / (float)src[IDX_RGBA_A] , SIZE_RGB);
    src[IDX_RGBA_A] = alpha;
    src += SIZE_RGBA;
  }
}

void alpha_opaque(unsigned char *src,
                  unsigned char opaqueness,
                  unsigned int n)
{
  src += IDX_RGBA_A; // offset
  while (n--)
  {
    *src = ((int)(*src * opaqueness) / 255);
    src+=SIZE_RGBA;
  }
}

void alpha_over(unsigned char *dst,
                const unsigned char *src1,
                const unsigned char *src2,
                unsigned int n)
{
  unsigned int b;
  unsigned int tmp;
  unsigned char w2;
  while (n--)
  {
    w2 = (255 - src1[IDX_RGBA_A]);
    for (b=0; b<SIZE_RGBA; ++b)
      dst[b] = src1[b] + INT_MULT(src2[b], w2, tmp);
//     dst[IDX_RGBA_A] = src1[IDX_RGBA_A] + INT_MULT(src2[IDX_RGBA_A], w2, tmp);
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}

void alpha_in(unsigned char *dst,
              const unsigned char *src1,
              const unsigned char *src2,
              unsigned int n)
{
  unsigned int b;
  unsigned int tmp;
  unsigned char w1;
  while (n--)
  {
    w1 = src2[IDX_RGBA_A];
    for (b=0; b<SIZE_RGBA; ++b)
      dst[b] = INT_MULT(src1[b], w1, tmp);
//     dst[IDX_RGBA_A] = INT_MULT(src1[b], w1, tmp);CLAMP0255( ((int)(src1[IDX_RGBA_A] * w1)) / 255 );
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}

void alpha_out(unsigned char *dst,
               const unsigned char *src1,
               const unsigned char *src2,
               unsigned int n)
{
  unsigned int b;
  unsigned int tmp;
  unsigned char w1;
  while (n--)
  {
    w1 = (255 - src2[IDX_RGBA_A]);
    for (b=0; b<SIZE_RGBA; ++b)
      dst[b] = INT_MULT(src1[b], w1, tmp);
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}

void alpha_atop(unsigned char *dst,
                const unsigned char *src1,
                const unsigned char *src2,
                unsigned int n)
{
  unsigned int b;
  unsigned int tmp;
  unsigned char w1, w2;
  while (n--)
  {
    w1 = src2[IDX_RGBA_A];
    w2 = (255 - src1[IDX_RGBA_A]);
    for (b=0; b<SIZE_RGB; ++b)
      dst[b] = INT_MULT(src1[b], w1, tmp) + INT_MULT(src2[b], w2, tmp);
    dst[IDX_RGBA_A] = w1;
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}

void alpha_xor(unsigned char *dst,
               const unsigned char *src1,
               const unsigned char *src2,
               unsigned int n)
{
  unsigned int b;
  unsigned int tmp;
  unsigned char w1, w2;
  while (n--)
  {
    w1 = (255 - src2[IDX_RGBA_A]);
    w2 = (255 - src1[IDX_RGBA_A]);
    for (b=0; b<SIZE_RGBA; ++b)
      dst[b] = INT_MULT(src1[b], w1, tmp) + INT_MULT(src2[b], w2, tmp);
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}
