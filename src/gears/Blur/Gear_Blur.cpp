/* Gear_Blur.cpp
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

#include "Gear_Blur.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

#include "Math.h"

extern "C" {
Gear* makeGear(Engine *engine, std::string name)
{
  return new Gear_Blur(engine,name);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Blur";
  return gearInfo;
}
}

Gear_Blur::Gear_Blur(Engine *engine, std::string name) : Gear(engine, "Blur", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(3, 0, 50)));
  _table = new SummedAreaTable<>();
}

Gear_Blur::~Gear_Blur()
{
  delete _table;
}

void Gear_Blur::init()
{
  //  _table->setImage(_VIDEO_IN->type());
}

bool Gear_Blur::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Blur::runVideo()
{ 
  _image = _VIDEO_IN->type();
  
  ASSERT_ERROR(_image);
  
  if (_image->isNull())
    return;
                  
  _sizeY = _image->height();
  _sizeX = _image->width();
  
  _outImage = _VIDEO_OUT->type();
  ASSERT_ERROR(_outImage);
      
  _outImage->resize(_sizeX, _sizeY);
    
  ////////////////////////////
  _blurSize=(int) MAX(_AMOUNT_IN->type()->value(), 0.0f);

  if (_blurSize == 0)
  {
    // No blur, just copy image.
    memcpy(_outImage->data(), _image->data(), _image->size()*sizeof(RGBA));
  }
  else
  {
    ASSERT_ERROR(_table);

    // Compute the summed area table.
    _table->reset((unsigned char*)_image->data(), _sizeX, _sizeY);
        
    _outData = (unsigned char*)_outImage->data();

    // Loop through the image's pixels.
    for (int y=0;y<_sizeY;y++)
    {
      for (int x=0;x<_sizeX;x++)
      {
        // The kernel's coordinates.
        _x1 = MAX(x - _blurSize - 1,-1);
        _x2 = MIN(x + _blurSize, _sizeX-1);
        _y1 = MAX(y - _blurSize - 1, -1);
        _y2 = MIN(y + _blurSize, _sizeY-1);

        // Get the sum in the current kernel.
        _table->getSum(_sum, _area, _x1, _y1, _x2, _y2);
        
        ASSERT_ERROR(_area >= 0);

        // Take the mean and copy it to output.
        divideVecVal(_outData, _sum, _area, SIZE_RGBA);

        // Increment iterator.
        _outData += SIZE_RGBA;
      }
    }
  }

}
