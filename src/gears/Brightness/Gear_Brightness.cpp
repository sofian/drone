/* Gear_Brightness.cpp
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

#include "Gear_Brightness.h"
#include "Engine.h"
#include "DroneMath.h"

#include <iostream>

#include "GearMaker.h"
#include "DroneMath.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_Brightness(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Brightness";
  gearInfo.classification = GearClassifications::video().color().instance();
  return gearInfo;
}
}

Gear_Brightness::Gear_Brightness(Schema *schema, std::string uniqueName) : Gear(schema, "Brightness", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  addPlug(_BRIGHTNESS_IN = new PlugIn<ValueType>(this, "Amount", false, new ValueType(0, -256, 256)));
}

Gear_Brightness::~Gear_Brightness()
{

}

void Gear_Brightness::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;
  
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  _size = (int)_image->size();

  if (_brightness != (int)_BRIGHTNESS_IN->type()->value())
  {
    _brightness = (int)_BRIGHTNESS_IN->type()->value();
    computeLookUpTable();
  }
  _imageIn  = (const unsigned char*) _image->data();
  _imageOut = (unsigned char*) _outImage->data();
    
  // Apply on image.
  for (int p=0; p<_size; ++p)
  {
    _imageOut[0] = _lut[_imageIn[0]];
    _imageOut[1] = _lut[_imageIn[1]];
    _imageOut[2] = _lut[_imageIn[2]];
    _imageOut[2] = _imageIn[3];

    _imageIn += SIZE_RGBA;
    _imageOut += SIZE_RGBA;
  }
  
}

void Gear_Brightness::internalInit()
{
  _brightness = 0;
  computeLookUpTable();
}

void Gear_Brightness::computeLookUpTable()
{
  // Compute lookup table.
  if (_brightness < 0)
  {
    for (int i=0; i<256; ++i)
      _lut[i] = CLAMP0255((i * (256 + _brightness))>>8);
  }
  else
  {
    for (int i=0; i<256; ++i)
      _lut[i] = CLAMP0255(i + (((256 - i) * _brightness)>>8));
  }
}

