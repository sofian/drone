/* Gear_GrayScale.cpp
 * Copyright (C) 2005 Jean-Sebastien Senecal
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

#include "Gear_FillAlpha.h"
#include "Engine.h"

#include "AlphaComposite.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_FillAlpha(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "FillAlpha";
  gearInfo.classification = GearClassifications::video().composition().instance();
  return gearInfo;
}
}

Gear_FillAlpha::Gear_FillAlpha(Schema *schema, std::string uniqueName) : Gear(schema, "FillAlpha", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_ALPHA_IN = new PlugIn<ValueType>(this, "Alpha", false, new ValueType(255, 0, 255)));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this,"ImgOUT", true));
}

Gear_FillAlpha::~Gear_FillAlpha()
{

}

void Gear_FillAlpha::runVideo()
{
  _image = _VIDEO_IN->type();
  
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  std::copy(_image->begin(), _image->end(), _outImage->begin());
  
  alpha_fill((unsigned char*)_outImage->data(), CLAMP( _ALPHA_IN->type()->intValue(), 0, 255 ), _outImage->size());
}
