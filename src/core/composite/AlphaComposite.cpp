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
#include "Math.h"
#include "ColorSpace.h"

void alpha_set(unsigned char *src,
               const unsigned char *mask,
               unsigned int n)
{
  src += IDX_RGBA_A; // offset
  while (n--)
  {
    *src = *mask++;
    src+=SIZE_RGBA;
  }
}

void alpha_fill(unsigned char *src,
                unsigned char alpha,
                unsigned int n)
{
  src += IDX_RGBA_A; // offset
  while (n--)
  {
    *src = alpha;
    src+=SIZE_RGBA;
  }  
}

void alpha_invert(unsigned char *src,
                  unsigned int n)                  
{
  src += IDX_RGBA_A; // offset
  while (n--)
  {
    *src ^= 0xff;
    src+=SIZE_RGBA;
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

void alpha_over(const unsigned char *src1,
                const unsigned char *src2,
                unsigned int n,
                unsigned char *dst)
{
  unsigned int b;
  unsigned char w2;
  while (n--)
  {
    w2 = (255 - src1[IDX_RGBA_A]);
    for (b=0; b<SIZE_RGB; ++b)
      dst[b] = CLAMP0255( src1[b] + src2[b] * w2 ); // XXX le clamping est pas necessaire, normalement...
    dst[IDX_RGBA_A] = src1[IDX_RGBA_A] + src2[IDX_RGBA_A] * w2;
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}

void alpha_in(const unsigned char *src1,
              const unsigned char *src2,
              unsigned int n,
              unsigned char *dst)
{
  unsigned int b;
  unsigned char w1;
  while (n--)
  {
    w1 = src2[IDX_RGBA_A];
    for (b=0; b<SIZE_RGB; ++b)
      dst[b] = CLAMP0255( src1[b] * w1 ); // XXX le clamping est pas necessaire, normalement...
    dst[IDX_RGBA_A] = CLAMP0255( src1[IDX_RGBA_A] * w1 );
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}

void alpha_out(const unsigned char *src1,
               const unsigned char *src2,
               unsigned int n,
               unsigned char *dst)
{
  unsigned int b;
  unsigned char w1;
  while (n--)
  {
    w1 = (255 - src2[IDX_RGBA_A]);
    for (b=0; b<SIZE_RGB; ++b)
      dst[b] = CLAMP0255( src1[b] * w1 ); // XXX le clamping est pas necessaire, normalement...
    dst[IDX_RGBA_A] = CLAMP0255( src1[IDX_RGBA_A] * w1 );
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}

void alpha_atop(const unsigned char *src1,
                const unsigned char *src2,
                unsigned int n,
                unsigned char *dst)
{
  unsigned int b;
  unsigned char w1, w2;
  while (n--)
  {
    w1 = src2[IDX_RGBA_A];
    w2 = (255 - src1[IDX_RGBA_A]);
    for (b=0; b<SIZE_RGB; ++b)
      dst[b] = CLAMP0255( src1[b] * w1 + src2[b] * w2 ); // XXX le clamping est pas necessaire, normalement...
    dst[IDX_RGBA_A] = w1;
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}

void alpha_xor(const unsigned char *src1,
               const unsigned char *src2,
               unsigned int n,
               unsigned char *dst)
{
  unsigned int b;
  unsigned char w1, w2;
  while (n--)
  {
    w1 = (255 - src2[IDX_RGBA_A]);
    w2 = (255 - src1[IDX_RGBA_A]);
    for (b=0; b<SIZE_RGB; ++b)
      dst[b] = CLAMP0255( src1[b] * w1 + src2[b] * w2 ); // XXX le clamping est pas necessaire, normalement...
    dst[IDX_RGBA_A] = src1[IDX_RGBA_A] * w1 + src2[IDX_RGBA_A] * w2;
    src1+=SIZE_RGBA;
    src2+=SIZE_RGBA;
    dst+=SIZE_RGBA;
  }
}
