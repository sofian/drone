#ifndef SUMMEDAREATABLE_INCLUDED
#define SUMMEDAREATABLE_INCLUDED

#include "VideoType.h"
#include "Utils.h"
#include "ColorSpace.h"

#include <iostream>

#include <assert.h>

class SummedAreaTable
{
public:

  SummedAreaTable();
  ~SummedAreaTable();

  void setImage(const MatrixType<RGBA> *image);

  void buildTable();
  void buildTableOfSquares();

  // *** il le faut pour les grayscales aussi...

  inline void getSum(RGBAint& sum, int& area, int x0, int y0, int x1, int y1) const;
  inline void getSumOfSquares(RGBAint& sumSquares, int& area, int x0, int y0, int x1, int y1) const;

  inline const RGBAint& getAcc(int x, int y) const;
  inline const RGBAint& getAccOfSquares(int x, int y) const;

  inline int getArea(int x0, int y0, int x1, int y1) const;

  static inline int total(RGBAint& val)
  {
    return sum((int*)&val, SIZE_RGB);
  }

public:

  RGBAint *_acc;

  RGBAint *_accSquares;

  const MatrixType<RGBA> *_image;

  const RGBA *_imageData;

  int _width, _height, _size;

  int _accR, _accG, _accB, _accA;
};

void SummedAreaTable::getSum(RGBAint& sum, int& area, int x0, int y0, int x1, int y1) const
{
  x0 = MAX(x0, -1);
  y0 = MAX(y0, -1);
  x1 = MIN(x1, _width-1);
  y1 = MIN(y1, _height-1);
  area = (x1-x0)*(y1-y0);

  // it is assumed that (x0,y0) <= (x1,y1)
  sum = _acc[y1 * _width + x1];

  if (x0 >= 0)
    sum -= _acc[y1 * _width + x0];
  if (y0 >= 0)
  {
    sum -= _acc[y0 * _width + x1];
    if (x0 >= 0)
      sum += _acc[y0 * _width + x0];
  }
  
}

void SummedAreaTable::getSumOfSquares(RGBAint& sumSquares, int &area, int x0, int y0, int x1, int y1) const
{
//   if (!sumSquares)
//     return;
  x0 = MAX(x0, -1);
  y0 = MAX(y0, -1);
  x1 = MIN(x1, _width-1);
  y1 = MIN(y1, _height-1);
  area = (x1-x0)*(y1-y0);

  // it is assumed that (x0,y0) <= (x1,y1)
  sumSquares = _accSquares[y1 * _width + x1];

  if (x0 >= 0)
    sumSquares -= _accSquares[y1 * _width + x0];
  if (y0 >= 0)
  {
    sumSquares -= _accSquares[y0 * _width + x1];
    if (x0 >= 0)
      sumSquares += _accSquares[y0 * _width + x0];
  }
}

int SummedAreaTable::getArea(int x0, int y0, int x1, int y1) const
{
  return( (MIN(x1,_width-1) - MAX(x0,-1))*(MIN(y1,_height-1) - MAX(y0,-1)) );
}

const RGBAint& SummedAreaTable::getAcc(int x, int y) const
{
  if (x < 0 || y < 0)
    return BLACK_RGBAint;
  else
    return _acc[y * _width + x];
}

const RGBAint& SummedAreaTable::getAccOfSquares(int x, int y) const
{
  if (x < 0 || y < 0)
    return BLACK_RGBAint;
  else
    return _accSquares[y * _width + x];
}

#endif
