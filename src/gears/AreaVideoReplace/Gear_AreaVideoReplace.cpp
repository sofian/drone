/* Gear_AreaVideoReplace.cpp
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

#include <iostream>
#include "Gear_AreaVideoReplace.h"
#include "Engine.h"

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AreaVideoReplace(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AreaVideoReplace";
  gearInfo.classification = GearClassifications::video().mask().instance();
  return gearInfo;
}
}

Gear_AreaVideoReplace::Gear_AreaVideoReplace(Schema *schema, std::string uniqueName) : Gear(schema, "AreaVideoReplace", uniqueName)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "VideoOut", true));

  addPlug(_REPLACE_AREA_IN = new PlugIn<AreaType>(this, "ReplaceArea", true));
  addPlug(_REPLACE_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ReplaceVideo", true));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "VideoIn", true));
}

Gear_AreaVideoReplace::~Gear_AreaVideoReplace()
{
}

void Gear_AreaVideoReplace::runVideo()
{
  _replaceArea = _REPLACE_AREA_IN->type();
  _imageReplaceIn = _REPLACE_VIDEO_IN->type();
  _imageIn = _VIDEO_IN->type();
  _imageOut = _VIDEO_OUT->type();

  _imageOut->resize(_imageIn->width(), _imageIn->height());
  
  ASSERT_ERROR(0 <= _replaceArea->x0() && _replaceArea->x0() + _replaceArea->width() <= _imageIn->width() &&
               0 <= _replaceArea->y0() && _replaceArea->y0() + _replaceArea->height() <= _imageIn->height());
  //  NOTICE("%d %d %d %d",  _replaceArea->width(),_imageReplaceIn->width(),_replaceArea->height(), _imageReplaceIn->height());
  ASSERT_ERROR(_replaceArea->width() == _imageReplaceIn->width() && _replaceArea->height() == _imageReplaceIn->height());
  
  int x = _replaceArea->x0();
  size_t rowWidth = (int)_replaceArea->width() * sizeof(RGBA);

  // Copy everything.
  memcpy(_imageOut->data(), _imageIn->data(), _imageIn->size() * sizeof(RGBA));

  // Copy the subwindow.
  for (int i=0, y=_replaceArea->y0(); i<(int)_replaceArea->height(); ++i, ++y)
    memcpy(&_imageOut->operator()(x,y), _imageReplaceIn->row(i), rowWidth);
}
