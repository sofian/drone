/* Gear_AreaArrayMask.cpp
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

#include <iostream>
#include "Gear_AreaArrayMask.h"
#include "Engine.h"

#include "GearMaker.h"


extern "C" {
Gear* makeGear()
{
  return new Gear_AreaArrayMask();
}


}

Gear_AreaArrayMask::Gear_AreaArrayMask() : Gear("AreaArrayMask")
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  addPlug(_WIDTH_IN = new PlugIn<ValueType>(this, "Width", false, new ValueType(352, 1, 1024)));
  addPlug(_HEIGHT_IN = new PlugIn<ValueType>(this, "Height", false, new ValueType(288, 1, 768)));
  addPlug(_AREA_ARRAY_IN = new PlugIn<AreaArrayType>(this, false, "AreaIN"));
}

Gear_AreaArrayMask::~Gear_AreaArrayMask()
{
}

void Gear_AreaArrayMask::runVideo()
{
  _areaArray = _AREA_ARRAY_IN->type();
  _sizeX = (int)MAX((int)_WIDTH_IN->type()->value(), 1);
  _sizeY = (int)MAX((int)_HEIGHT_IN->type()->value(), 1);
  //  NOTICE("Size = %d x %d", _sizeX, _sizeY);
  
  _imageOut = _VIDEO_OUT->type();
  _imageOut->resize((size_t)_sizeX, (size_t)_sizeY);
  
  _imageOut->fill(BLACK_RGBA);
  for (AreaArrayType::const_iterator it = _areaArray->begin(); it != _areaArray->end(); ++it)
  {
    ASSERT_ERROR(it->x0 <= it->x1 && it->y0 <= it->y1);
    _minX = MAX((int)it->x0,0);
    _maxX = MIN((int)it->x1,_sizeX-1);
    _minY = MAX((int)it->y0,0);
    _maxY = MIN((int)it->y1,_sizeY-1);
    _rowWidth = _maxX - _minX;
    //  NOTICE("minX = %d, maxX = %d, minY = %d, maxY = %d, rowBytes = %d", minX, maxX, minY, maxY, rowBytes);
    for (int y = _minY; y < _maxY; ++y)
      fill(&_imageOut->row(y)[_minX], WHITE_RGBA, _rowWidth); // copy row
  }
}


