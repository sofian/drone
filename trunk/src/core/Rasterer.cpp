/* Rasterer.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
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

#include "Rasterer.h"
#include "Utils.h"
#include <iostream>

Rasterer::Rasterer() : _image(NULL), _imageData(NULL), _brush(NULL), _rasterMode(RASTER_REPLACE)
{
  _color.r=255;
  _color.g=255;
  _color.b=255;
  _color.a=255;
}

Rasterer::~Rasterer()
{
}

void Rasterer::setImage(Array2DType<RGBA> *image)
{
  _image=image;

  if (_image!=NULL)
    _imageData = _image->data();

}

void Rasterer::setColor(unsigned char r, unsigned char g, unsigned char b)
{
  _color.r=r;
  _color.g=g;
  _color.b=b;
}

void Rasterer::line(int x0, int y0, int x1, int y1)
{
}

void Rasterer::rect(int x0, int y0, int x1, int y1, bool filled)
{
  unsigned char *data;
  unsigned char r=_color.r;
  unsigned char g=_color.g;
  unsigned char b=_color.b;

  if (filled)
    for (int y=y0;y<y1;y++)
    {
      data = (unsigned char*) &_image->get(x0,y);
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
    data = (unsigned char*) &_image->get(x0,y0);
    for (int x=x0;x<=x1;x++)
    {
      *(data++) = r;
      *(data++) = g;
      *(data++) = b;
      data++;
    }

    for (int y=y0; y<=y1; ++y)
    {
      data = (unsigned char*) &_image->get(x0,y);
      *(data++) = r;
      *(data++) = g;
      *(data++) = b;       

      data = (unsigned char*) &_image->get(x1,y);
      *(data++) = r;
      *(data++) = g;
      *(data++) = b;
    }

    data = (unsigned char*) &_image->get(x0,y1);
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
    //    int radx2 = SQR(radx);
    int rady2 = SQR(rady);

    for (int y=rady; y>=0; --y)
    {
      int xmax = (int) (radx * sqrt(1 - (float)SQR(y) / (float)rady2));
      for (int x=xmax; x>=0; --x)
        ellipsePoints(x0, y0, x, y);
    }
  } else
  {
    double d2;
    int x = 0;
    int y = rady;

    int radx2 = SQR(radx);
    int rady2 = SQR(rady);

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

    d2 = rady2 * SQR(x + 0.5) + ( SQR(y - 1.) - rady2) * radx2;
    while (y > 0)
    {
      if (d2 < 0)
      {
        d2 += rady2 * (2*x + 2) + radx2 * (-2*y + 3);
        x++;
      } else
        d2 += radx2 * (-2* y + 3);
      y--;
      ellipsePoints(x0, y0, x, y);
    }
  }
}

void Rasterer::fill(int x0, int y0)
{
}



