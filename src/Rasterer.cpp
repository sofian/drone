#include "Rasterer.h"
#include "Utils.h"
#include <iostream>

Rasterer::Rasterer() : _canvas(NULL), _canvasData(NULL), _brush(NULL), _rasterMode(RASTER_REPLACE)
{
  _color.R=255;
  _color.G=255;
  _color.B=255;
  _color.A=255;
}

Rasterer::~Rasterer()
{
}

void Rasterer::setCanvas(Canvas *canvas)
{
  _canvas=canvas;

  if (_canvas!=NULL)
    _canvasData = _canvas->_data;
    
}

void Rasterer::setColor(unsigned char r, unsigned char g, unsigned char b)
{
  _color.R=r;
  _color.G=g;
  _color.B=b;
}

void Rasterer::line(int x0, int y0, int x1, int y1)
{
}

void Rasterer::rect(int x0, int y0, int x1, int y1, bool filled)
{
  int sizeX = _canvas->sizeX();
  unsigned char *data;
  unsigned char r=_color.R;
  unsigned char g=_color.G;
  unsigned char b=_color.B;

  if (filled)
    for (int y=y0;y<y1;y++)
    {    
      data = (unsigned char*)&_canvasData[y*sizeX + x0];
      for (int x=x0;x<x1;x++)
      {
        *(data++) = r;
        *(data++) = g;
        *(data++) = b;            
        data++;
      }
    }
  else
  {
    data = (unsigned char*)&_canvasData[y0*sizeX + x0];
    for (int x=x0;x<=x1;x++)
    {
      *(data++) = r;
      *(data++) = g;
      *(data++) = b;
      data++;
    }

    for (int y=y0; y<=y1; ++y)
    {
      data = (unsigned char*)&_canvasData[y*sizeX + x0];
      *(data++) = r;
      *(data++) = g;
      *(data++) = b;       

      data = (unsigned char*)&_canvasData[y*sizeX + x1];
      *(data++) = r;
      *(data++) = g;
      *(data++) = b;
    }
      
    data = (unsigned char*)&_canvasData[y1*sizeX + x0];
    for (int x=x0;x<=x1;x++)
    {
      *(data++) = r;
      *(data++) = g;
      *(data++) = b;            
      data++;
    }
      
  }
}

void Rasterer::ellipse(int x0, int y0, int radx, int rady, bool filled)
{
  if (filled)
  {
    int radx2 = square(radx);
    int rady2 = square(rady);
    
    for (int y=rady; y>=0; --y)
    {
      int xmax = (int) radx * sqrt(1 - (float)square(y) / (float)rady2);
      for (int x=xmax; x>=0; --x)
        ellipsePoints(x0, y0, x, y);
    }
  }
  else
  {
    double d2;
    int x = 0;
    int y = rady;
    
    int radx2 = square(radx);
    int rady2 = square(rady);
    
    double d1 = rady2 + (0.25 - rady) * radx2;
    ellipsePoints(x0, y0, x, y);
    
    while ( radx2 * (y - 0.5) > rady2 * (x + 1) )
    {
      if (d1 < 0)
        d1 += rady2 * (2*x + 3);
      else
      {
        d1 += rady2 * (2*x + 3) + radx2 * (-2*y + 2);
        y--;
      }
      x++;
      ellipsePoints(x0, y0, x, y);
    }
    
    d2 = rady2 * square(x + 0.5) + ( square(y - 1.) - rady2) * radx2;
    while (y > 0)
    {
      if (d2 < 0) {
        d2 += rady2 * (2*x + 2) + radx2 * (-2*y + 3);
        x++;
      }
      else
        d2 += radx2 * (-2* y + 3);
      y--;
      ellipsePoints(x0, y0, x, y);
    }
  }
}

void Rasterer::fill(int x0, int y0)
{
}



