#ifndef SUMMEDAREATABLE_INCLUDED
#define SUMMEDAREATABLE_INCLUDED

#include "Canvas.h"
#include "Utils.h"

#include <iostream>

#include <assert.h>

class SummedAreaTable
{
  static const RGBAint ZERO;
public:

  SummedAreaTable();
  ~SummedAreaTable();

  void setCanvas(Canvas *canvas);

  void buildTable();
  void buildTableOfSquares();

  // *** il le faut pour les grayscales aussi...

  inline void getSum(RGBAint *sum, int x0, int y0, int x1, int y1) const;
  inline void getSumOfSquares(RGBAint *sumSquares, int x0, int y0, int x1, int y1) const;

  inline const RGBAint& getAcc(int x, int y) const;
  inline const RGBAint& getAccOfSquares(int x, int y) const;
  
  inline int getArea(int x0, int y0, int x1, int y1) const;

  static inline int total(RGBAint *sum)
  {
    return sum->R + sum->G + sum->B;
  }

public:
  
  RGBAint *_acc;

  RGBAint *_accSquares;

  Canvas *_canvas;

  RGBA *_canvasData;

  int _sizeX, _sizeY, _size;

  int _accR, _accG, _accB, _accA;
  
};

void SummedAreaTable::getSum(RGBAint *sum, int x0, int y0, int x1, int y1) const
{
  if (!sum)
    return;

  memset(sum, 0, sizeof(RGBAint)); // reset
  
  // it is assumed that (x0,y0) <= (x1,y1)

  *sum += _acc[y1 * _sizeX + x1];
  
  if (x0 >= 0)
    *sum -= _acc[y1 * _sizeX + x0];
  if (y0 >= 0)
  {
    *sum -= _acc[y0 * _sizeX + x1];
    if (x0 >= 0)
      *sum += _acc[y0 * _sizeX + x0];
  }
}

void SummedAreaTable::getSumOfSquares(RGBAint *sumSquares, int x0, int y0, int x1, int y1) const
{
  if (!sumSquares)
    return;

  memset(sumSquares, 0, sizeof(RGBAint)); // reset
  
  // it is assumed that (x0,y0) <= (x1,y1)

  *sumSquares += _accSquares[y1 * _sizeX + x1];
  
  if (x0 >= 0)
    *sumSquares -= _accSquares[y1 * _sizeX + x0];
  if (y0 >= 0)
  {
    *sumSquares -= _accSquares[y0 * _sizeX + x1];
    if (x0 >= 0)
      *sumSquares += _accSquares[y0 * _sizeX + x0];
  }
}

int SummedAreaTable::getArea(int x0, int y0, int x1, int y1) const
{
  if (x0 < 0) x0 = 0;
  if (x1 < 0) x1 = 0;
  if (y0 < 0) y0 = 0;
  if (y1 < 0) y1 = 0;
  
  if (x0 >= _sizeX) x0 = _sizeX-1;
  if (x1 >= _sizeX) x1 = _sizeX-1;
  if (y0 >= _sizeY) y0 = _sizeY-1;
  if (y1 >= _sizeY) y1 = _sizeY-1;

  return ((x1-x0) * (y1-y0));
}

const RGBAint& SummedAreaTable::getAcc(int x, int y) const
{
  if (x < 0 || y < 0)
    return ZERO;
  else
    return _acc[y * _sizeX + x];
}

const RGBAint& SummedAreaTable::getAccOfSquares(int x, int y) const
{
  if (x < 0 || y < 0)
    return ZERO;
  else
    return _accSquares[y * _sizeX + x];
}

#endif
