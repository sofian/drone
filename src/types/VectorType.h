/* VectorType.h
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

#ifndef __VECTORTYPE_INCLUDED
#define __VECTORTYPE_INCLUDED

#include <vector>
#include "ValueType.h"

template <class T>
class VectorType : public AbstractType, public std::vector<T>
{
public:
  VectorType(int size = 0,
             T fillValue = T())
    : _fillValue(fillValue)
  {
    addSubType(_typeSize);
    resize(size);
    std::fill(begin(), end(), _fillValue);
  }

  void resize(size_t size)
  {
    std::vector<T>::resize(size);
    _typeSize.setValue(size);
  }

  void fill(const T& fillValue)
  {
    std::fill(begin(), end(), fillValue);
  }

  const T* data() const {return &front();}
  T* data() {return &front();}
  

  virtual ~VectorType() {}
  
protected:
  ValueType _typeSize;
  T _fillValue;
};

#endif //__VECTORTYPE_INCLUDED


