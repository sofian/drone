/* Gear_Gamma.cpp
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

#include "Gear_Gamma.h"
#include "Engine.h"
#include "Math.h"

#include <iostream>

#include "GearMaker.h"
#include "Math.h"
#include <float.h>

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_Gamma(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Gamma";
  gearInfo.classification = GearClassifications::video().color().instance();
  return gearInfo;
}
}
                  
                  
Gear_Gamma::Gear_Gamma(Schema *schema, std::string uniqueName) : Gear(schema, "Gamma", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  addPlug(_GAMMA_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(1, 0, 10)));
}

Gear_Gamma::~Gear_Gamma()
{

}

bool Gear_Gamma::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Gamma::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;
  
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  _size = (int)_image->size();

  float gamma = MAX(_GAMMA_IN->type()->value(), FLT_MIN);

  _imageIn  = (const unsigned char*) _image->data();
  _imageOut = (unsigned char*) _outImage->data();

  // Compute lookup table.
  _lut[0] = 0;
  float invGamma = 1.0 / gamma;
  for (int i=1; i<256; ++i)
    _lut[i] = CLAMP0255( ROUND(255.0f * exp( invGamma * log( float(i) / 255.0f ) ) ));

  // Apply on image.
  for (int p=0; p<_size; ++p)
  {
    _imageOut[0] = _lut[_imageIn[0]];
    _imageOut[1] = _lut[_imageIn[1]];
    _imageOut[2] = _lut[_imageIn[2]];

    _imageIn += SIZE_RGBA;
    _imageOut += SIZE_RGBA;
  }
  
}
