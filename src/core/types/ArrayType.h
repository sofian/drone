/* ArrayType.h
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

#ifndef ARRAYTYPE_INCLUDED
#define ARRAYTYPE_INCLUDED

#include "Array.h"
#include "ValueType.h"

template <class T>
class ArrayType : public AbstractType, public Array<T>
{
public:
  typedef typename Array<T>::size_type size_type;

  ArrayType(int size = 0,
						T fillValue = T()) :
		AbstractType()
  {
    addSubType(_typeSize);
    resize(size);
    Array<T>::fill(fillValue);
  }

  virtual ~ArrayType() {}

  TYPE_BASE_METHODS(ArrayType<T>)

  /**
   * Resizes the array to given new size.
   *
   * @param size the new size of the array
   */
  void resize(size_type size)
  {
    Array<T>::resize(size);
    _typeSize.setValue(size);
  }

protected:
  ValueType _typeSize;
};

#endif // ARRAYTYPE_INCLUDED


