/* Gear_AreaVideoSelect.cpp
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
#include "Gear_AreaVideoSelect.h"
#include "Engine.h"

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AreaVideoSelect(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AreaVideoSelect";
  gearInfo.classification = GearClassifications::video().mask().instance();
  return gearInfo;
}
}

Gear_AreaVideoSelect::Gear_AreaVideoSelect(Schema *schema, std::string uniqueName) : Gear(schema, "AreaVideoSelect", uniqueName)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "VideoOut", true));
  addPlug(_SELECT_IN = new PlugIn<AreaType>(this, "Select", true));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "Video", true));
}

Gear_AreaVideoSelect::~Gear_AreaVideoSelect()
{
}

void Gear_AreaVideoSelect::runVideo()
{
  _area = _SELECT_IN->type();
  _imageIn = _VIDEO_IN->type();
  _imageOut = _VIDEO_OUT->type();

  _imageOut->resize(_area->width(), _area->height());
  ASSERT_ERROR(0 <= _area->x0() && _area->x0() + _area->width() <= _imageIn->width() &&
               0 <= _area->y0() && _area->y0() + _area->height() <= _imageIn->height());

  int x = _area->x0();
  size_t rowWidth = (int)_area->width() * sizeof(RGBA);

  for (int i=0, y=_area->y0(); i<(int)_area->height(); ++i, ++y)
    memcpy(_imageOut->row(i), &_imageIn->operator()(x,y), rowWidth);
}
