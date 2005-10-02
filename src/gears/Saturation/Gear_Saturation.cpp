/* Gear_Saturation.cpp
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

#include "Gear_Saturation.h"
#include "Engine.h"

#include "Utils.h"

#include <iostream>

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_Saturation(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Saturation";
  gearInfo.classification = GearClassifications::video().color().instance();
  return gearInfo;
}
}

Gear_Saturation::Gear_Saturation(Schema *schema, std::string uniqueName) : Gear(schema, "Saturation", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  addPlug(_SATURATION_IN = new PlugIn<ValueType>(this, "Amount", false, new ValueType(1.0f, 0.0f, 10.0f)));

}

Gear_Saturation::~Gear_Saturation()
{

}

void Gear_Saturation::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  _size = (int) _image->size();

  if (_saturation != _SATURATION_IN->type()->value())
  {
      _saturation = _SATURATION_IN->type()->value();
      computeLookUpTable();
  }
  _imageIn  = (const unsigned char*) _image->data();
  _imageOut = (unsigned char*) _outImage->data();

  float oneMinusAmount = 1.0f-_saturation;
  int rwgt = (int)(19595 * oneMinusAmount);
  int gwgt = (int)(38470 * oneMinusAmount);
  int bwgt = (int)(7471  * oneMinusAmount);

  int bw=0;

  if (0 <= _saturation && _saturation <=1) // optimisation: no clamping needed
  {
    for (int p=0; p<_size; ++p)
    {
      _r = _imageIn[0];
      _g = _imageIn[1];
      _b = _imageIn[2];
      
      bw = (_r*rwgt + _g*gwgt + _b*bwgt) >> 16;
      
      _imageOut[0] = (unsigned char) (bw + _lut[_r]);
      _imageOut[1] = (unsigned char) (bw + _lut[_g]);
      _imageOut[2] = (unsigned char) (bw + _lut[_b]);
      
      _imageIn+=SIZE_RGBA;
      _imageOut+=SIZE_RGBA;
    }
  }
  else
  {
    for (int p=0; p<_size; ++p)
    {
      _r = _imageIn[0];
      _g = _imageIn[1];
      _b = _imageIn[2];
    
      bw = (_r*rwgt + _g*gwgt + _b*bwgt) >> 16;

      _imageOut[0] = (unsigned char) CLAMP0255(bw + _lut[_r]);
      _imageOut[1] = (unsigned char) CLAMP0255(bw + _lut[_g]);
      _imageOut[2] = (unsigned char) CLAMP0255(bw + _lut[_b]);
    
      _imageIn+=SIZE_RGBA;
      _imageOut+=SIZE_RGBA;
    }
  }

}

void Gear_Saturation::internalInit()
{
  _saturation = 0.0f;
  computeLookUpTable();
}

void Gear_Saturation::computeLookUpTable()
{
  // Compute lookup table.
  for (int i=0; i<256; ++i)
    _lut[i] = (int) (i*_saturation);
}
