#ifndef SUMMEDAREATABLE_INCLUDED
#define SUMMEDAREATABLE_INCLUDED

#include "Canvas.h"
#include "Utils.h"
#include "ColorSpace.h"

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

  static inline int total(RGBAint *val)
  {
    return sum((int*)val, SIZE_RGB);
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

  // it is assumed that (x0,y0) <= (x1,y1)
  *sum = _acc[y1 * _sizeX + x1];

  if (x0 >= 0)
    subtract((int*)sum, (int*)&_acc[y1 * _sizeX + x0], SIZE_RGBA);
  if (y0 >= 0)
  {
    subtract((int*)sum, (int*)&_acc[y0 * _sizeX + x1], SIZE_RGBA);
    if (x0 >= 0)
      add((int*)sum, (int*)&_acc[y0 * _sizeX + x0], SIZE_RGBA);
  }
}

void SummedAreaTable::getSumOfSquares(RGBAint *sumSquares, int x0, int y0, int x1, int y1) const
{
  if (!sumSquares)
    return;

  // it is assumed that (x0,y0) <= (x1,y1)
  *sumSquares = _accSquares[y1 * _sizeX + x1];
  
  if (x0 >= 0)
    subtract((int*)sumSquares, (int*)&_accSquares[y1 * _sizeX + x0], SIZE_RGBA);
  if (y0 >= 0)
  {
    subtract((int*)sumSquares, (int*)&_accSquares[y0 * _sizeX + x1], SIZE_RGBA);
    if (x0 >= 0)
      add((int*)sumSquares, (int*)&_accSquares[y0 * _sizeX + x0], SIZE_RGBA);
  }
}

int SummedAreaTable::getArea(int x0, int y0, int x1, int y1) const
{
  int maxX = _sizeX-1;
  int maxY = _sizeY-1;
  return ( (CLAMP(x1,0,maxX) - CLAMP(x0,0,maxX))*(CLAMP(y1,0,maxY) - CLAMP(y0,0,maxY)) );
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
