#ifndef SUMMEDAREATABLE_INCLUDED
#define SUMMEDAREATABLE_INCLUDED

#include "Matrix.h"
#include "Math.h"
#include "Utils.h"
#include "ColorSpace.h"

#include <iostream>

#include <assert.h>

template <typename Type = unsigned char, typename AccType = int, size_t SIZE = SIZE_RGBA>
class SummedAreaTable
{
public:
  SummedAreaTable()
    : _table(0), _acc(), _width(0), _height(0), _size(0)
  {
    NOTICE("SIZE = %d, sizeof(AccType) = %d", SIZE, sizeof(AccType));
    clear(_black, SIZE);
  }

  ~SummedAreaTable() {}

  void setTable(const Type *table, size_t width, size_t height);
  
  void buildTable();
  
  inline void getSum(AccType *sum, int& area, int x0, int y0, int x1, int y1) const;
  
  inline const AccType* getAcc(int x, int y) const;

  inline int getArea(int x0, int y0, int x1, int y1) const;
  
public:

  const Type *_table;
  
  Matrix<AccType> _acc;

  int _width, _height, _size;

  AccType _tmpAcc[SIZE];

  AccType _black[SIZE];
};

template <typename Type, typename AccType, size_t SIZE>
inline void SummedAreaTable<Type, AccType, SIZE>::getSum(AccType *sum, int& area, int x0, int y0, int x1, int y1) const
{
  ASSERT_ERROR(sum);
  
  x0 = MAX(x0, -1);
  y0 = MAX(y0, -1);
  x1 = MIN(x1, _width-1);
  y1 = MIN(y1, _height-1);
  area = (x1-x0)*(y1-y0);

  ASSERT_ERROR(area >= 0);
  
  // it is assumed that (x0,y0) <= (x1,y1)
  memcpy(sum, _acc.row(y1) + SIZE*x1, SIZE*sizeof(AccType));
//   sum = _acc[y1 * _width + x1];

  if (x0 >= 0)
    subtractAccVecVec(sum, _acc.row(y1) + SIZE*x0, SIZE);
  //    sum -= _acc[y1 * _width + x0];
  if (y0 >= 0)
  {
    subtractAccVecVec(sum, _acc.row(y0) + SIZE*x1, SIZE);
    //    sum -= _acc[y0 * _width + x1];
    if (x0 >= 0)
      addAccVecVec(sum, _acc.row(y0) + SIZE*x0, SIZE);
      //sum += _acc[y0 * _width + x0];
  }
  
}

// void SummedAreaTable::getSumOfSquares(RGBAint& sumSquares, int &area, int x0, int y0, int x1, int y1) const
// {
// //   if (!sumSquares)
// //     return;
//   x0 = MAX(x0, -1);
//   y0 = MAX(y0, -1);
//   x1 = MIN(x1, _width-1);
//   y1 = MIN(y1, _height-1);
//   area = (x1-x0)*(y1-y0);

//   // it is assumed that (x0,y0) <= (x1,y1)
//   sumSquares = _accSquares[y1 * _width + x1];

//   if (x0 >= 0)
//     sumSquares -= _accSquares[y1 * _width + x0];
//   if (y0 >= 0)
//   {
//     sumSquares -= _accSquares[y0 * _width + x1];
//     if (x0 >= 0)
//       sumSquares += _accSquares[y0 * _width + x0];
//   }
// }

template <typename Type, typename AccType, size_t SIZE>
inline int SummedAreaTable<Type, AccType, SIZE>::getArea(int x0, int y0, int x1, int y1) const
{
  ASSERT_ERROR_MESSAGE((MIN(x1,_width-1) - MAX(x0,-1))*(MIN(y1,_height-1) - MAX(y0,-1)) >= 0, "Area is negative, something is wrong.");
  return( (MIN(x1,_width-1) - MAX(x0,-1))*(MIN(y1,_height-1) - MAX(y0,-1)) );
}

template <typename Type, typename AccType, size_t SIZE>
inline const AccType* SummedAreaTable<Type, AccType, SIZE>::getAcc(int x, int y) const
{
  if (x < 0 || y < 0)
    return _black;
  else
    return _acc.row(y) + SIZE*x;
}

template <typename Type, typename AccType, size_t SIZE>
void SummedAreaTable<Type, AccType, SIZE>::setTable(const Type *table, size_t width, size_t height)
{
  _table = table;
  if (_table)
  {
    _width = width;
    _height = height;
    _size = width*height;
  }
}

template <typename Type, typename AccType, size_t SIZE>
void SummedAreaTable<Type, AccType, SIZE>::buildTable()
{
  if (!_table)
    return;

  int rowWidth = _width * SIZE;

  _acc.resize(rowWidth, _height);
  
  register Type *iterData = (Type*) _table;
  register AccType *iterAcc  = (AccType*) &_acc.front();

  // upper row
  // copy(_tmpAcc, iterData, rowWidth);
//   copy(iterAcc, _tmpAcc,  rowWidth);
//   iterData += rowWidth;
//   iterAcc  += rowWidth;

  clear(_tmpAcc, SIZE);
  for (int x=0; x<_width; ++x)
  {
    addAccVecVec(_tmpAcc, iterData, SIZE);
    copy(iterAcc, _tmpAcc, SIZE);
//     _accR += (int)*(iterData++);
//     _accG += (int)*(iterData++);
//     _accB += (int)*(iterData++);
//     _accA += (int)*(iterData++);
//     *(iterAcc++) = _accR;
//     *(iterAcc++) = _accG;
//     *(iterAcc++) = _accB;
//     *(iterAcc++) = _accA;
    iterData+=SIZE;
    iterAcc +=SIZE;
  }

  // other rows
  for (int y=1; y<_height; ++y)
  {
    // copy upper line
    memcpy(iterAcc, iterAcc - rowWidth, rowWidth * sizeof(AccType));
    clear(_tmpAcc, SIZE);
    
//     addAccVecVec(_tmpAcc, iterData, rowWidth);
//     addAccVecVec(iterAcc, _tmpAcc,  rowWidth);
    
    for (int x=0; x<_width; ++x)
    {
      addAccVecVec(_tmpAcc, iterData, SIZE);
      addAccVecVec(iterAcc, _tmpAcc,  SIZE);
      iterData+=SIZE;
      iterAcc +=SIZE;
//       _accR += (int)*(iterData++);
//       _accG += (int)*(iterData++);
//       _accB += (int)*(iterData++);
//       _accA += (int)*(iterData++);
//       *(iterAcc++) += _accR;
//       *(iterAcc++) += _accG;
//       *(iterAcc++) += _accB;
//       *(iterAcc++) += _accA;
    }
  }
}

// void SummedAreaTable::buildTableOfSquares()
// {
//   if (!_table)
//     return;

//   // check if we need to resize
//   if ((int)(_table->width() * _table->height()) != _size) // there was a change
//   {
//     // resize
//     _width = _table->width();
//     _height = _table->height();
//     _size = _width * _height;
//   }

//   // reallocate
//   _accSquares = (RGBAint*)realloc(_accSquares, _size*sizeof(RGBAint));

//   _tableData = _table->data();

//   register unsigned char *iterData = (unsigned char*) _tableData;
//   register int *iterAcc = (int*) _acc;

//   int rowWidth = _width * 4;

//   // upper row
//   _accR = _accG = _accB = _accA = 0;
//   for (int x=0; x<_width; ++x)
//   {
//     _accR += SQR( (int)*(iterData++) );
//     _accG += SQR( (int)*(iterData++) );
//     _accB += SQR( (int)*(iterData++) );
//     _accA += SQR( (int)*(iterData++) );
//     *(iterAcc++) = _accR;
//     *(iterAcc++) = _accG;
//     *(iterAcc++) = _accB;
//     *(iterAcc++) = _accA;
//   }

//   // other rows
//   for (int y=1; y<_height; ++y)
//   {
//     // copy upper line
//     memcpy(iterAcc, iterAcc - rowWidth, rowWidth * sizeof(int));
//     _accR = _accG = _accB = _accA = 0;
//     for (int x=0; x<_width; ++x)
//     {
//       _accR += SQR( (int)*(iterData++) );
//       _accG += SQR( (int)*(iterData++) );
//       _accB += SQR( (int)*(iterData++) );
//       _accA += SQR( (int)*(iterData++) );
//       *(iterAcc++) += _accR;
//       *(iterAcc++) += _accG;
//       *(iterAcc++) += _accB;
//       *(iterAcc++) += _accA;
//     }
//   }
// }


// const RGBAint& SummedAreaTable::getAccOfSquares(int x, int y) const
// {
//   if (x < 0 || y < 0)
//     return BLACK_RGBAint;
//   else
//     return _accSquares[y * _width + x];
// }

#endif
