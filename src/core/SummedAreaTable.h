/* SummedAreaTable.h
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

#ifndef SUMMEDAREATABLE_INCLUDED
#define SUMMEDAREATABLE_INCLUDED

#include "Array2D.h"
#include "Math.h"
#include "Utils.h"
#include "ColorSpace.h"

#include <iostream>

#include <assert.h>

/**
 * A summed area table is a general utility for many image based algorithms. It takes in input
 * a table of <code>width x height</code> elements, each of which is made of <code>SIZE</code> of
 * type <code>Type</code>. Each cell <code>s(x,y)</code> of the summed area table contains the
 * sum of all points in the rectangle <code>(0,0) , (x,y)</code> of the original image. These
 * values are computed using a very efficient (dynamical programming) algorithm, linear in the
 * number of cells. The resulting table allows one to calculate the sum of the values in a
 * sub-window with only four additions.
 *
 * @author Jean-Sébastien Senécal
 * @version %I% %G%
 */
template <typename Type = unsigned char, typename AccType = int, size_t SIZE = SIZE_RGBA>
class SummedAreaTable : public Array2D<AccType*>
{
public:
  // using (as suggested by Norm)
  using Array2D<AccType*>::get;
  using Array2D<AccType*>::width;
  using Array2D<AccType*>::height;
  using Array2D<AccType*>::size;
  using Array2D<AccType*>::resize;
  using Array2D<AccType*>::front;
  using Array2D<AccType*>::_width;
  using Array2D<AccType*>::_height;

  //! Default constructor.
  SummedAreaTable() : _acc(), _cellSize(SIZE*sizeof(AccType)) {}

  //! Destructor.
  ~SummedAreaTable() {}

  /**
   * Computes the summed area table from a source image.
   *
   * @param src the source image (should be a contiguous array of size <code>width*height*SIZE*sizeof(Type)</code>
   * @param srcWidth the number of horizontal cells
   * @param srcHeight the number of vertical cells
   */
  void reset(const Type *srcTable, size_t srcWidth, size_t srcHeight);

  /**
   * Quickly computes the sum of the cells in a sub-window. Also computes the total area (number of cells) of the
   * sub-window. The sub-window is specified by passing a pair of points <code>((x0,y0),(x1,y1))</code>. The method
   * assumes that <code>x0 <= x1</code> and that <code>y0 <= y1</code>.
   *
   * @param sum the sum of the cells in the sub-window
   * @param area the number of cells in the sub-window
   * @param x0 the horizontal coordinate of the upper-left corner of the sub-window
   * @param y0 the vertical coordinate of the upper-left corner of the sub-window
   * @param x1 the horizontal coordinate of the bottom-right corner of the sub-window
   * @param y1 the vertical coordinate of the bottom-right corner of the sub-window
   * @see SummedAreaTable::reset(const Type*,size_t,size_t)
   */
  inline void getSum(AccType *sum, int& area, int x0, int y0, int x1, int y1) const;

  /**
   * Returns the sum of all pixels in the sub-window defined by <code>((0,0),(x,y))</code>.
   *
   * @param x the horizontal coordinate of the bottom-right corner of the sub-window
   * @param y the vertical coordinate of the bottom-right corner of the sub-window
   * @return the sum of the pixels in the sub-window
   */
  inline const AccType* getAcc(int x, int y) const;

  /**
   * Just returns the area of a sub-window.
   *
   * @param x0 the horizontal coordinate of the upper-left corner of the sub-window
   * @param y0 the vertical coordinate of the upper-left corner of the sub-window
   * @param x1 the horizontal coordinate of the bottom-right corner of the sub-window
   * @param y1 the vertical coordinate of the bottom-right corner of the sub-window
   * @return the number of cells in the sub-window
   */
  static inline int getArea(int x0, int y0, int x1, int y1);

public:
  // The accumulation matrix.
  Array2D<AccType> _acc;

  // Internal use.
  int _srcWidth;  // width of the source image (= width() - 1)
  int _srcHeight; // height of the source image (= height() - 1)

  AccType _accBuffer[SIZE]; // accumulation buffer

  int _rowWidth; // = SIZE * width()
  size_t _cellSize; // = SIZE * sizeof(AccType)
};

// Implementations.

template <typename Type, typename AccType, size_t SIZE>
inline void SummedAreaTable<Type, AccType, SIZE>::getSum(AccType *sum, int& area, int x0, int y0, int x1, int y1) const
{
  ASSERT_ERROR(sum);
  ASSERT_ERROR(-1 <= x0 && x0 <= x1 && x1 < _srcWidth);
  ASSERT_ERROR(-1 <= y0 && y0 <= y1 && y1 < _srcHeight);

  area = (x1-x0)*(y1-y0);

  ASSERT_ERROR(area >= 0);

  x0++; x1++;
  y0++; y1++;

  // it is assumed that (x0,y0) <= (x1,y1)
  memcpy(sum, get(x1,y1), SIZE*sizeof(AccType));

  if (x0 >= 0)
    subtractAccVecVec(sum, get(x0,y1), SIZE);
  if (y0 >= 0)
  {
    subtractAccVecVec(sum, get(x1,y0), SIZE);
    if (x0 >= 0)
      addAccVecVec(sum, get(x0,y0), SIZE);
  }
}

template <typename Type, typename AccType, size_t SIZE>
inline int SummedAreaTable<Type, AccType, SIZE>::getArea(int x0, int y0, int x1, int y1)
{
  ASSERT_ERROR(-1 <= x0 && x0 <= x1 && x1 < _width);
  ASSERT_ERROR(-1 <= y0 && y0 <= y1 && y1 < _height);
  ASSERT_ERROR_MESSAGE((x1-x0)*(y1-y0), "Area is negative, something is wrong.");
  return( (x1-x0)*(y1-y0) );
}

template <typename Type, typename AccType, size_t SIZE>
inline const AccType* SummedAreaTable<Type, AccType, SIZE>::getAcc(int x, int y) const
{
  ASSERT_ERROR(-1 <= x && x < _srcWidth);
  ASSERT_ERROR(-1 <= y && y < _srcHeight);
  return get(x+1, y+1);
}

template <typename Type, typename AccType, size_t SIZE>
void SummedAreaTable<Type, AccType, SIZE>::reset(const Type *src, size_t srcWidth, size_t srcHeight)
{
  if (!src)
  {
    WARNING("Void table provided, returning without processing.");
    return;
  }

  register Type *iterData;
  register AccType *iterAcc;
  register size_t i;

  // Resize.
  _srcWidth = srcWidth;
  _srcHeight = srcHeight;
  _rowWidth = (_srcWidth+1) * SIZE;

  resize(_srcWidth+1, _srcHeight+1); // resize this table of pointers to structure
  _acc.resize(_rowWidth, _srcHeight+1); // resize accumulation table

  // Initialize pointers to structure.
  AccType **iter = &front();
  iterAcc        = &_acc.front();
  for (i=0; i<size(); ++i)
  {
    *iter++ = iterAcc;
    iterAcc += SIZE;
  }

  // Init iterators.
  iterData = (Type*) src;
  iterAcc  = &_acc.front();

  // Process first row (all zeros).
  ::clear(iterAcc, _rowWidth);
  iterAcc += _rowWidth;

  if (_srcHeight >= 0)
  {
    // Process second row.
    memset(_accBuffer, 0, _cellSize);
    memset(iterAcc,    0, _cellSize); // first column is void
    iterAcc += SIZE;
    for (size_t x=1; x<width(); ++x)
    {
      for (i=0; i<SIZE; ++i)
        *iterAcc++ = (_accBuffer[i] += *iterData++);
    }

    // Process other rows.
    for (size_t y=2; y<height(); ++y)
    {
      // Copy upper line.
      memcpy(iterAcc, iterAcc - _rowWidth, _rowWidth * sizeof(AccType));

      // Process row.
      memset(_accBuffer, 0, _cellSize);
      memset(iterAcc,    0, _cellSize); // first column is void
      iterAcc += SIZE;
      for (size_t x=1; x<width(); ++x)
      {
        for (i=0; i<SIZE; ++i)
          *iterAcc++ += (_accBuffer[i] += *iterData++);
      }
    }
  }
}

#endif
