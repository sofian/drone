/* MatrixType.h
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

#ifndef __MATRIXTYPE_INCLUDED
#define __MATRIXTYPE_INCLUDED

#include "Matrix.h"
#include "ValueType.h"

template <class T>
class MatrixType : public AbstractType, public Matrix<T>
{
public:
  typedef typename Matrix<T>::size_type size_type;

  MatrixType(int width = 0,
             int height = 0, 
             T fillValue = T())
    : Matrix<T>(width, height)      
  {
    addSubType(_typeWidth);
    addSubType(_typeHeight);
    resize(width, height);
    std::fill(begin(), end(), fillValue);
  }

  void resize(size_type size)
  {
    Matrix<T>::resize(size);
  }

  void resize(size_type width, size_type height)
  {
    Matrix<T>::resize(width, height);
    _typeWidth.setValue(width);
    _typeHeight.setValue(height);
  }

  void fill(const T& fillValue)
  {
    std::fill(begin(), end(), fillValue);
  }

  const T* data() const {return &front();}
  T* data() {return &front();}

  virtual ~MatrixType() {}
  

protected:
  ValueType _typeWidth;
  ValueType _typeHeight;  
};

#endif //__MATRIXTYPE_INCLUDED


