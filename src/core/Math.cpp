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

#include "Math.h"

float sinLut[FASTMATH_TABLESIZE];
float cosLut[FASTMATH_TABLESIZE];
float sqrtLut[FASTMATH_TABLESIZE];
float FASTMATH_LOOKUP = FASTMATH_TABLESIZE/(float)TWICE_PI;

void initMath()
{
	for (int n=0; n<FASTMATH_TABLESIZE; n++)
	{
		float a=n*(TWICE_PI/(float)FASTMATH_TABLESIZE);
		sinLut[n]=sin(a);
		cosLut[n]=cos(a);
    sqrtLut[n]=sqrt(a);
	}
}

