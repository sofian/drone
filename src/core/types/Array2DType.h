/* Array2DType.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef ARRAY2DTYPE_INCLUDED
#define ARRAY2DTYPE_INCLUDED

#include "Array2D.h"
#include "ValueType.h"

template <class T>
class Array2DType : public AbstractType, public Array2D<T>
{
public:
  typedef typename Array2D<T>::size_type size_type;

  using Array2D<T>::resize;

  Array2DType(int width = 0,
             int height = 0, 
             T fillValue = T())
    : AbstractType(), 
	  Array2D<T>(width, height)
  {
    addSubType(_typeWidth);
    addSubType(_typeHeight);
    resize(width, height);
    Array2D<T>::fill(fillValue);
  }

  TYPE_BASE_METHODS(Array2DType<T>)

  virtual void resize(size_type size)
  {
    Array2D<T>::resize(size);
  }

  virtual void resize(size_type width, size_type height)
  {
    Array2D<T>::resize(width, height);
    _typeWidth.setValue(width);
    _typeHeight.setValue(height);
  }

  virtual ~Array2DType() {}

protected:
  ValueType _typeWidth;
  ValueType _typeHeight;  
};

#endif // ARRAY2DTYPE_INCLUDED


