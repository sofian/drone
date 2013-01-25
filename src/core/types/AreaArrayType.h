/* AreaArrayType.h
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

#ifndef AREAARRAYTYPE_INCLUDED
#define AREAARRAYTYPE_INCLUDED

#include "ArrayType.h"
#include "Engine.h"

/**
 * This structure represents an area on an image, namely, a sub-window.
 * For now, it is a simple rectangle that is specified by givin two
 * points.
 */
struct Area
{
  /**
   * Default constructor. Initializes all points to origin (0,0) (void area).
   */
  Area() : x0(0), y0(0), width(0), height(0) {}

  //! The upper-left point.
  int x0, y0;

  //! The width and height
  unsigned int width, height;
};

/**
 * This type represents a list of areas on an image. For the moment,
 * areas are represented as rectangles (sub-windows).
 *
 * @see Area
 * @author Jean-Sébastien Senécal
 * @version %I% %G%
 */
class AreaArrayType : public ArrayType<Area>
{
public:
  /**
   * Default constructor
   *
   * @param size the number of areas in the vector
   */
  AreaArrayType(int size = 0) : 
		ArrayType<Area>(size)
  {
  }

  //! Destructor.
  virtual ~AreaArrayType() {}

  TYPE_BASE_METHODS(AreaArrayType, (221, 111, 33))
};


#endif 
