/* Array2D.h
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

#ifndef ARRAY2D_INCLUDED
#define ARRAY2D_INCLUDED

#include "error.h"
#include "Array.h"

/**
 * This class is a general-purpose structure for accessing data structured as
 * a two-dimensional array. The memory it contains is contiguous as the class
 * derives from Array, which is a subclass of the STL <code>vector</code> class.
 * The main advantage of using a <code>Array2D</code> over an <code>Array</code>
 * is that it offers a nice interface for accessing data in a "by-row" fashion.
 *
 * @author Jean-Sébastien Senécal, Mathieu Guindon
 * @version %I% %G%
 */
template <class T>
class Array2D : public Array<T>
{
public:
  //! Inner type definitions.
  typedef typename std::vector<T>::size_type size_type;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::reference reference;
  typedef typename std::vector<T>::pointer pointer;

public:
  // using (as suggested by Norm)
  using Array<T>::data;
  using Array<T>::size;
	using Array<T>::end;
	using Array<T>::begin;
  /**
   * Default constructor.
   */
  Array2D() : _width(0), _height(0) {}

  /**
   * Copy constructor.
   */
  Array2D(const Array2D<T>& copy) : Array<T>(copy)
  {
    resize(copy.width(), copy.height());
    // Not necessary: this is accomplished by the call to Array<T>(copy)
    //std::copy(std::vector<T>::begin(), std::vector<T>::end(), copy.begin());
  }

  /**
   * Copy constructor.
   */
  Array2D(const Array<T>& copy)
  {
    resize(copy.size());
    std::copy(Array2D<T>::begin(), Array2D<T>::end(), copy.begin());
  }
  
  /**
   * Constructor.
   *
   * @param width the wished number of columns
   * @param height the wished number of rows
   */
  Array2D(size_type width,
          size_type height)    
    : _width(0), _height(0)
  {
    resize(width, height);
  }

  //! Destructor.
  virtual ~Array2D() {}

  /**
   * Returns the number of rows.
   *
   * @return the height
   */
  size_type height() const { return _height; }

  /**
   * Returns the number of columns.
   *
   * @return the width
   */
  size_type width() const { return _width; }

  /**
   * Returns a reference to the value at coordinates <code>(x,y)</code>.
   *
   * @param x the column index
   * @param y the row index
   * @return the value
   */
  reference operator()(size_type x, size_type y) const
  {
    ASSERT_ERROR(x < width());
    ASSERT_ERROR(y < height());
    return _rows[y][x];
  }

  /**
   * Returns a reference to the value at coordinates <code>(x,y)</code>.
   *
   * @param x the column index
   * @param y the row index
   * @return the value
   */
  reference get(size_type x, size_type y) const
  {
    ASSERT_ERROR(x < width());
    ASSERT_ERROR(y < height());
    return _rows[y][x];
  }

  /**
   * Returns a reference to the vector of rows.
   *
   * @return the reference to the row vector
   */
  std::vector<pointer>& rows() const { return _rows; }

  /**
   * Resizes the matrix as a vector. Calling this sets <code>height</code> to <code>1</code>
   * and <code>width</code> to <code>size</code>.
   *
   * @param size the new size
   */
  void resize(size_type size)
  {
    std::vector<T>::resize(size);
    _width=size;
    _height=1;

    _rows.resize(1);
    _rows[0] = data();
  }

  /**
   * Resizes the matrix to new width/height.
   *
   * @param width the new width
   * @param height the new height
   */
  void resize(size_type width, size_type height)
  {
    if (width==_width && height==_height)
      return;
    
    resize(width*height);
    _width = width;
    _height = height;
  
    // Efficient setting of the rows pointer.
    _rows.resize(height);
    pointer iterData = data();
    for (typename std::vector<pointer>::iterator it = _rows.begin();
          it != _rows.end(); ++it, iterData += width)
      *it = iterData;
  }               

  /**
   * Returns a pointer to the first element of a row (const version).
   *
   * @param y the row number
   * @return a pointer to the start of the row
   */
  const pointer row(size_type y) const
  {
    ASSERT_ERROR(y < height());
    return _rows[y];
  }

  /**
   * Returns a pointer to the first element of a row.
   *
   * @param y the row number
   * @return a pointer to the start of the row
   */
  pointer row(size_type y)
  {
    ASSERT_ERROR(y < height());
    return _rows[y];
  }

protected:
  //! The number of columns.
  size_type _width;

  //! The number of rows
  size_type _height;

  //! The precomputed array of row pointers.
  std::vector<pointer> _rows;
};

#endif
