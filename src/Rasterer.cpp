#include "Rasterer.h"
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

void Rasterer::circle(int x0, int y0, int radius, bool filled)
{
}

void Rasterer::fill(int x0, int y0)
{
}



