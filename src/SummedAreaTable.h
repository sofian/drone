#ifndef SUMMEDAREATABLE_INCLUDED
#define SUMMEDAREATABLE_INCLUDED

#include "Matrix.h"
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
class SummedAreaTable
{
public:
  //! Default constructor.
  SummedAreaTable()
    : _acc(), _width(0), _height(0)
  {
    clear(_zero, SIZE);
  }

  //! Destructor.
  ~SummedAreaTable() {}

  /**
   * Computes the summed area table from a source image.
   *
   * @param table the source image (should be a contiguous array of size <code>width*height*SIZE*sizeof(Type)</code>
   * @param width the number of horizontal cells
   * @param height the number of vertical cells
   */
  void buildTable(const Type *table, size_t width, size_t height);

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
   * @see SummedAreaTable::buildTable(const Type*,size_t,size_t)
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

  inline int getArea(int x0, int y0, int x1, int y1) const;
  
public:
  //! The matrix containing the sums.
  Matrix<AccType> _acc;

  // Internal use.
  int _width;
  int _height;
  int _rowWidth;
  AccType _tmpAcc[SIZE];
  AccType _zero[SIZE]; // a void value
};

// Implementations.

template <typename Type, typename AccType, size_t SIZE>
inline void SummedAreaTable<Type, AccType, SIZE>::getSum(AccType *sum, int& area, int x0, int y0, int x1, int y1) const
{
  ASSERT_ERROR(sum);
  ASSERT_ERROR(-1 <= x0 && x0 <= x1 && x1 < _width);
  ASSERT_ERROR(-1 <= y0 && y0 <= y1 && y1 < _height);
  
  area = (x1-x0)*(y1-y0);
  
  ASSERT_ERROR(area >= 0);
  
  // it is assumed that (x0,y0) <= (x1,y1)
  memcpy(sum, _acc.row(y1) + SIZE*x1, SIZE*sizeof(AccType));

  if (x0 >= 0)
    subtractAccVecVec(sum, _acc.row(y1) + SIZE*x0, SIZE);
  if (y0 >= 0)
  {
    subtractAccVecVec(sum, _acc.row(y0) + SIZE*x1, SIZE);
    if (x0 >= 0)
      addAccVecVec(sum, _acc.row(y0) + SIZE*x0, SIZE);
  }
  
}

template <typename Type, typename AccType, size_t SIZE>
inline int SummedAreaTable<Type, AccType, SIZE>::getArea(int x0, int y0, int x1, int y1) const
{
  ASSERT_ERROR(-1 <= x0 && x0 <= x1 && x1 < _width);
  ASSERT_ERROR(-1 <= y0 && y0 <= y1 && y1 < _height);
  ASSERT_ERROR_MESSAGE((x1-x0)*(y1-y0), "Area is negative, something is wrong.");
  return( (x1-x0)*(y1-y0) );
}

template <typename Type, typename AccType, size_t SIZE>
inline const AccType* SummedAreaTable<Type, AccType, SIZE>::getAcc(int x, int y) const
{
  ASSERT_ERROR(x < _width);
  ASSERT_ERROR(y < _height);
  if (x < 0 || y < 0)
    return _zero;
  else
    return _acc.row(y) + SIZE*x;
}

template <typename Type, typename AccType, size_t SIZE>
void SummedAreaTable<Type, AccType, SIZE>::buildTable(const Type *table, size_t width, size_t height)
{
  if (!table)
  {
    WARNING("Void table provided, returning without precessing.");
    return;
  }

  // Resize.
  _width = width;
  _height = height; 
  _rowWidth = _width * SIZE;
  
  _acc.resize(_rowWidth, _height); // Resize accumulation table

  // Init iterators.
  register Type *iterData = (Type*) table;
  register AccType *iterAcc  = (AccType*) &_acc.front();
  register size_t i;

  // Process upper row.
  clear(_tmpAcc, SIZE);
  for (int x=0; x<_width; ++x)
  {
    for (i=0; i<SIZE; ++i)
      *iterAcc++ = (_tmpAcc[i] += *iterData++);
  }

  // Process other rows.
  for (int y=1; y<_height; ++y)
  {
    // Copy upper line.
    memcpy(iterAcc, iterAcc - _rowWidth, _rowWidth * sizeof(AccType));

    // Process row.
    clear(_tmpAcc, SIZE);
    for (int x=0; x<_width; ++x)
    {
      for (i=0; i<SIZE; ++i)
        *iterAcc++ += (_tmpAcc[i] += *iterData++);
    }
  }
}

#endif
