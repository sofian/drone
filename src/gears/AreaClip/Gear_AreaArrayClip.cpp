/* Gear_AreaArrayClip.cpp
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
#include "Gear_AreaArrayClip.h"
#include "Engine.h"
#include "Math.h"

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AreaArrayClip(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AreaArrayClip";
  gearInfo.classification = GearClassifications::video().mask().instance();
  return gearInfo;
}
}

Gear_AreaArrayClip::Gear_AreaArrayClip(Schema *schema, std::string uniqueName) : Gear(schema, "AreaArrayClip", uniqueName)
{    
  addPlug(_AREA_ARRAY_OUT = new PlugOut<AreaArrayType>(this, "AreaOut", true));
  addPlug(_AREA_ARRAY_IN = new PlugIn<AreaArrayType>(this, "AreaIn", true));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "Video", true));
}

Gear_AreaArrayClip::~Gear_AreaArrayClip()
{
}

void Gear_AreaArrayClip::runVideo()
{
  _areaArrayIn = _AREA_ARRAY_IN->type();
  _areaArrayOut = _AREA_ARRAY_OUT->type();

  _areaArrayOut->resize(_areaArrayIn->size());

  _sizeX = _VIDEO_IN->type()->width();
  _sizeY = _VIDEO_IN->type()->height();
  
  AreaArrayType::const_iterator in = _areaArrayIn->begin();
  AreaArrayType::iterator out = _areaArrayOut->begin();
  
  for ( ; in != _areaArrayIn->end(); ++in, ++out)
  {
    out->x0 = MAX(in->x0, 0);
    out->y0 = MAX(in->y0, 0);
    out->width = MIN((int)in->width, _sizeX - in->x0);
    out->height = MIN((int)in->height, _sizeY - in->y0);
  }
}


