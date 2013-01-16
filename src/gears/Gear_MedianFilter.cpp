/* Gear_MedianFilter.cpp
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#include "Gear_MedianFilter.h"

#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_MedianFilter, Gear_MedianFilter, "MedianFilter")

Gear_MedianFilter::Gear_MedianFilter() : Gear("MedianFilter")
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Amount", false, new ValueType(1, 0, 7)));
}

Gear_MedianFilter::~Gear_MedianFilter()
{
}

void Gear_MedianFilter::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;


  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _sizeY = _image->height();
  _sizeX = _image->width();

  _data = (unsigned char*)_image->data();    
  _outData = (unsigned char*)_outImage->data();

  _medianSize = (int) CLAMP((int)_AMOUNT_IN->type()->value(), 0, 7);

  ////////////////////////////

  for (int y=0;y<_sizeY;y++)
  {
    for (int x=0;x<_sizeX;x++)
    {
      _x1 = x - _medianSize;
      _x2 = x + _medianSize;
      _y1 = y - _medianSize;
      _y2 = y + _medianSize;

      _x1 = CLAMP(_x1,0,_sizeX-1);
      _x2 = CLAMP(_x2,0,_sizeX-1);
      _y1 = CLAMP(_y1,0,_sizeY-1);
      _y2 = CLAMP(_y2,0,_sizeY-1);
      
//       if (_x1 < 0)_x1 = 0;
//       if (_x2 < 0)_x2 = 0;
//       if (_y1 < 0)_y1 = 0;
//       if (_y2 < 0)_y2 = 0;

//       if (_x1 >= _sizeX)_x1 = _sizeX-1;
//       if (_x2 >= _sizeX)_x2 = _sizeX-1;
//       if (_y1 >= _sizeY)_y1 = _sizeY-1;
//       if (_y2 >= _sizeY)_y2 = _sizeY-1;

      int nCols = _y2-_y1+1;
      int nRows = _x2-_x1+1;
      int nextRow = (_sizeX-nCols)*4;
      unsigned char *startIterData = _data + (_y1*_sizeX+_x1)*4;

      for (int z=0; z<4; ++z)
      {
        _iterMedianSelect = _medianSelect;
        //{
          _iterData = startIterData + z;
          for (int ySub=_y1; ySub<=_y2; ++ySub)
          {
            for (int xSub=_x1; xSub<=_x2; ++xSub)
            {
              *_iterMedianSelect++ = *_iterData;
              _iterData+=4;
            }
            _iterData += nextRow;
          }
          //}
        *_outData++ = quickSelect(_medianSelect, nRows*nCols);
      }

    }

  }


}
