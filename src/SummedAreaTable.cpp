#include "SummedAreaTable.h"
#include <iostream>

#include <assert.h>

SummedAreaTable::SummedAreaTable()
  : _acc(0), _accSquare(0), _canvas(0), _canvasData(0), _sizeX(0), _sizeY(0), _size(0)
{
}

SummedAreaTable::~SummedAreaTable()
{
  if (_acc)
    delete[] _acc;
  if (_accSquare)
    delete[] _accSquare;
}

void SummedAreaTable::setCanvas(Canvas *canvas)
{
  _canvas = canvas;
  if (_canvas)
  {
    _canvasData = _canvas->_data;
    _sizeX = _canvas->sizeX();
    _sizeY = _canvas->sizeY();
    _size = _sizeX * _sizeY;

    _acc = new RGBAint[_size];
  }
}

void SummedAreaTable::buildTable()
{
  if (!_canvas)
    return;
  
  // check if we need to resize
  if (_canvas->sizeX() * _canvas->sizeY() != _size) // there was a change...
  {
    // resize
    _sizeX = _canvas->sizeX();
    _sizeY = _canvas->sizeY();
    _size = _sizeX * _sizeY;

    // reallocate
    // *** vérifier si on peut utiliser realloc ...
    if (_acc)
      delete[] _acc;
    _acc = new RGBAint[_size];
  }

  _canvasData = _canvas->_data;
  
  unsigned char *iterData = (unsigned char*) _canvasData;
  int *iterAcc = (int*) _acc;

  int rowWidth = _sizeX * 4;
  
  // upper row
  _accR = _accG = _accB = _accA = 0;
  for (int x=0; x<_sizeX; ++x)
  {
    _accR += (int)*(iterData++);
    _accG += (int)*(iterData++);
    _accB += (int)*(iterData++);
    _accA += (int)*(iterData++);
    *(iterAcc++) = _accR;
    *(iterAcc++) = _accG;
    *(iterAcc++) = _accB;
    *(iterAcc++) = _accA;
  }

  // other rows
  for (int y=1; y<_sizeY; ++y)
  {
    // copy upper line
    memcpy(iterAcc, iterAcc - rowWidth, rowWidth * sizeof(int));
    _accR = _accG = _accB = _accA = 0;
    for (int x=0; x<_sizeX; ++x)
    {
      _accR += (int)*(iterData++);
      _accG += (int)*(iterData++);
      _accB += (int)*(iterData++);
      _accA += (int)*(iterData++);
      *(iterAcc++) += _accR;
      *(iterAcc++) += _accG;
      *(iterAcc++) += _accB;
      *(iterAcc++) += _accA;
    }
  }

//   for (int y=0; y<_sizeY; ++y)
//   {
//     for (int x=0; x<_sizeX; ++x)
//     {
//       RGBAint acc = _acc[y*_sizeX+x];
//       RGBA val = _canvasData[y*_sizeX+x];
// //       unsigned char r = _canvasData[y*_sizeX*4+x];
// //       unsigned char g = _canvasData[y*_sizeX*4+x+1];
// //       unsigned char b = _canvasData[y*_sizeX*4+x+1];
// //       unsigned char a = _canvasData[y*_sizeX*4+x+1];
//       printf("(%d %d %d %d)=",val.R, val.G, val.B, val.A);
//       printf("(%d %d %d %d) ",acc.R, acc.G, acc.B, acc.A);
//     }
//     printf("\n");
//   }

//   for (int y=0; y<_sizeY; ++y)
//   {
//     for (int x=0; x<_sizeX; ++x)
//       if (y==0)
        
//       if (_acc[y*_sizeX+x]
//       }  
}

void SummedAreaTable::buildTable2()
{

}

void SummedAreaTable::getSum(RGBAint *sum, int x0, int y0, int x1, int y1)
{
  getSumIn(sum, _acc, x0, y0, x1, y1);
}

void SummedAreaTable::getSumSquare(RGBAint *sumSquare, int x0, int y0, int x1, int y1)
{
  getSumIn(sumSquare, _accSquare, x0, y0, x1, y1);
}

int SummedAreaTable::getArea(int x0, int y0, int x1, int y1)
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


void SummedAreaTable::getSumIn(RGBAint *sum, RGBAint *acc, int x0, int y0, int x1, int y1)
{
  if (!sum)
    return;

  memset(sum, 0, sizeof(RGBAint)); // reset
  
  // it is assumed that (x0,y0) <= (x1,y1)

  // check upper bounds
  if (x0 >= _sizeX) x0 = _sizeX-1;
  if (x1 >= _sizeX) x1 = _sizeX-1;
  if (y0 >= _sizeY) y0 = _sizeY-1;
  if (y1 >= _sizeY) y1 = _sizeY-1;

  if (x0 < 0) x0 = 0;
  if (x1 < 0) x1 = 0;
  if (y0 < 0) y0 = 0;
  if (x1 < 0) y1 = 0;

  addRGBAint(sum, &acc[y1 * _sizeX + x1]);
  subRGBAint(sum, &acc[y1 * _sizeX + x0]);
  subRGBAint(sum, &acc[y0 * _sizeX + x1]);
  addRGBAint(sum, &acc[y0 * _sizeX + x0]);
  
//   if (! (sum->R >= 0 && sum->G >= 0 && sum->B >=0 && sum->A >= 0 ) )
//   {
//     std::cout << "[" << _sizeX << "," << _sizeY << "] (" << x0 << "," << y0 << ") (" << x1 << "," << y1 << ")" << std::endl;
//   }
      
//   // calculate it efficiently
//   if (x0 < 0)
//   {
//     if (y0 >= 0)
//       subRGBAint(sum, &acc[y0 * _sizeX + x1]);
//   }
//   else if (y0 < 0)
//   {
//     subRGBAint(sum, &acc[y1 * _sizeX + x0]);
//   }
//   else
//   {
//     addRGBAint(sum, &acc[y0 * _sizeX + x0]);
//     subRGBAint(sum, &acc[y1 * _sizeX + x0]);
//     subRGBAint(sum, &acc[y0 * _sizeX + x1]);
//   }

//   addRGBAint(sum, &acc[y1 * _sizeX + x1]);
  
}
