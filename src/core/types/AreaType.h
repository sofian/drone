/* AreaType.h
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

#ifndef AREATYPE_INCLUDED
#define AREATYPE_INCLUDED

#include "AbstractType.h"
#include "Engine.h"

/**
 * This type represents a list of areas on an image. For the moment,
 * areas are represented as rectangles (sub-windows).
 *
 * @see Area
 * @author Jean-Sébastien Senécal
 * @version %I% %G%
 */
class AreaType : public AbstractType
{
public:
  static const QString TYPENAME;
  
  /**
   * Default constructor
   */
  AreaType(int x0=0, int y0=0, size_t width=0, size_t height=0)
    : AbstractType(), _x0(x0), _y0(y0), _width(width), _height(height)
  {
  }

  //! Destructor.
  virtual ~AreaType() {}

  TYPE_BASE_METHODS(AreaType, (221,46,33))

  void setOrigin(int x0, int y0) { _x0 = x0; _y0 = y0; }
  void resize(size_t width, size_t height) { _width = width; _height = height; }
  
  int x0() const { return _x0; }
  int y0() const { return _y0; }
  size_t width() const { return _width; }
  size_t height() const { return _height; }

  void debug() {
    NOTICE("Area: %d %d %d %d", _x0, _y0, _width, _height);
  }
  
protected:
  //! The upper-left point.
  int _x0, _y0;

  //! The width and height
  size_t _width, _height;
};

#endif 
