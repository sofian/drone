/* Gear_Rescale.cpp
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

#include "Gear_Rescale.h"
#include "Engine.h"

#include "Array2DType.h"

#include <iostream>

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_Rescale(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Rescale";
  gearInfo.classification = GearClassifications::video().distortion().instance();
  return gearInfo;
}
}

Gear_Rescale::Gear_Rescale(Schema *schema, std::string uniqueName) : Gear(schema, "Rescale", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT",true ));
  addPlug(_WIDTH_IN = new PlugIn<ValueType>(this, "Width", false, new ValueType(352, 1, 1024)));
  addPlug(_HEIGHT_IN = new PlugIn<ValueType>(this, "Height", false, new ValueType(288, 1, 768)));
}

Gear_Rescale::~Gear_Rescale()
{

}

void Gear_Rescale::runVideo()
{
  _image = _VIDEO_IN->type();
  _outImage = _VIDEO_OUT->type();
  _newWidth = (int) MAX((int)_WIDTH_IN->type()->value(), 1);
  _newHeight = (int) MAX((int)_HEIGHT_IN->type()->value(), 1);
  _outImage->resize(_newWidth, _newHeight);

  rescale_image(_outImage->data(), _image->data(), _newWidth, _newHeight, _image->width(), _image->height());
}
