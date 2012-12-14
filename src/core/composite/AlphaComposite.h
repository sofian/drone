/* AlphaComposite.h
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

#ifndef ALPHACOMPOSITE_INCLUDED
#define ALPHACOMPOSITE_INCLUDED

#include "DroneMath.h"
#include "ColorSpace.h"

// XXX penser à la possibilité de mettre des RGBA

// Precomputing alpha operations /////////////////////////////////////////////

void alpha_premultiply(unsigned char *src,
                      unsigned int n);

void alpha_demultiply(unsigned char *src,
                      unsigned int n);  

// Unary operators ///////////////////////////////////////////////////////////

//! Sets the alpha channel of #src# to the values specified in #mask#.
void alpha_set(unsigned char *src,
               const unsigned char *mask,
               unsigned int n);

//! Fills the alpha channel of #src# with the alpha value specified by #alpha#.
void alpha_fill(unsigned char *src,
                unsigned char alpha,
                unsigned int n);

//! Inverts the alpha channel of #src#.
void alpha_invert(unsigned char *src,
                  unsigned int n);

//! Opaqueness operator.
void alpha_opaque(unsigned char *src,
                  unsigned char opaqueness,
                  unsigned int n);

// Binary operators //////////////////////////////////////////////////////////

//! Alpha compositing "over" operation.
void alpha_over(unsigned char *dst,
                const unsigned char *src1,
                const unsigned char *src2,
                unsigned int n);

//! Alpha compositing "in" operation.
void alpha_in(unsigned char *dst,
              const unsigned char *src1,
              const unsigned char *src2,
              unsigned int n);

//! Alpha compositing "out" operation.
void alpha_out(unsigned char *dst,
               const unsigned char *src1,
               const unsigned char *src2,
               unsigned int n);

//! Alpha compositing "atop" operation.
void alpha_atop(unsigned char *dst,
                const unsigned char *src1,
                const unsigned char *src2,
                unsigned int n);

//! Alpha compositing "xor" operation.
void alpha_xor(unsigned char *dst,
               const unsigned char *src1,
               const unsigned char *src2,
               unsigned int n);

#endif
