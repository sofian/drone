#ifndef SUMMEDAREATABLE_INCLUDED
#define SUMMEDAREATABLE_INCLUDED

#include "Canvas.h"

class SummedAreaTable
{
public:

  SummedAreaTable();
  ~SummedAreaTable();

  void setCanvas(Canvas *canvas);

  void buildTable();
  void buildTable2();

  void getSum(RGBAint *sum, int x0, int y0, int x1, int y1);
  void getSumSquare(RGBAint *sumSquare, int x0, int y0, int x1, int y1);
  int getArea(int x0, int y0, int x1, int y1);

private:
  void getSumIn(RGBAint *sum, RGBAint *acc, int x0, int y0, int x1, int y1);

  static inline void addRGBAint(RGBAint *val, RGBAint *inc)
  {
    int *valIter = (int*)val;
    int *incIter = (int*)inc;
    for (int i=0; i<4; ++i)
      *(valIter++) += (*incIter++);
  }

  static inline void subRGBAint(RGBAint *val, RGBAint *inc)
  {
    int *valIter = (int*)val;
    int *incIter = (int*)inc;
    for (int i=0; i<4; ++i)
      *(valIter++) -= (*incIter++);
  }

public:
  
  RGBAint *_acc;
  //int *_accGray;

  RGBAint *_accSquare;
  //int *_accGray2;

  Canvas *_canvas;

  RGBA *_canvasData;

  int _sizeX, _sizeY, _size;

  int _accR, _accG, _accB, _accA;
  
};

#endif
