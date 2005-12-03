/* Rasterer.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef RASTERER_INCLUDED
#define RASTERER_INCLUDED

#include "Array2DType.h"
#include "ColorSpace.h"

class Rasterer
{
public:
  enum RasterMode
  {
    RASTER_REPLACE, RASTER_ADD, RASTER_SUB, RASTER_AND, RASTER_OR, RASTER_XOR
  };

  Rasterer();
  ~Rasterer();

  void setImage(Array2DType<RGBA> *image);    
  void setBrush(Array2DType<RGBA> *brush){_brush=brush;};
  void setColor(const RGBA &color){_color=color;};
  void setColor(unsigned char r, unsigned char g, unsigned char b);

  void setRasterMode(RasterMode rasterMode){_rasterMode=rasterMode;};

  void line(int x0, int y0, int x1, int y1);
  void rect(int x0, int y0, int x1, int y1, bool filled);
  void ellipse(int x0, int y0, int radx, int rady, bool filled);

  void ellipsePoints(int x0, int y0, int relx, int rely)
  {
    int x    = x0 + relx;
    int xSym = x0 - relx;
    int y    = y0 + rely;
    int ySym = y0 - rely;
    _image->get(y,x)       = _color;
    _image->get(y,xSym)    = _color;
    _image->get(ySym,x)    = _color;
    _image->get(ySym,xSym) = _color;
  }

private:

  Array2DType<RGBA> *_image;
  RGBA *_imageData;
  Array2DType<RGBA> *_brush; //!if brush = null then color is use to raster
  RGBA _color;

  RasterMode _rasterMode;


};

#endif
