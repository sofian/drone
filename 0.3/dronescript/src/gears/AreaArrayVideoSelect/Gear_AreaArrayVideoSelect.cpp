/* Gear_AreaArrayVideoSelect.cpp
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
#include "Gear_AreaArrayVideoSelect.h"
#include "Engine.h"

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AreaArrayVideoSelect(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AreaArrayVideoSelect";
  gearInfo.classification = GearClassifications::video().mask().instance();
  return gearInfo;
}
}

Gear_AreaArrayVideoSelect::Gear_AreaArrayVideoSelect(Schema *schema, std::string uniqueName) : Gear(schema, "AreaArrayVideoSelect", uniqueName)
{    
  addPlug(_VIDEO_LIST_OUT = new PlugOut<ListType>(this, "VideoList", true));
  addPlug(_SELECT_IN = new PlugIn<AreaArrayType>(this, "Select", true));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "Video", true));
}

Gear_AreaArrayVideoSelect::~Gear_AreaArrayVideoSelect()
{
}

void Gear_AreaArrayVideoSelect::runVideo()
{
  _areaArray = _SELECT_IN->type();
  _imageIn = _VIDEO_IN->type();

  _listOut = _VIDEO_LIST_OUT->type();
  _listOut->resize(_areaArray->size());
  
  ListType::iterator vit = _listOut->begin();
  for (AreaArrayType::const_iterator it = _areaArray->begin(); it != _areaArray->end(); ++it, ++vit)
  {
    //    ASSERT_ERROR(it->x0 <= it->x1 && it->y0 <= it->y1);
    ASSERT_ERROR(0 <= it->x0 && it->x0 + it->width <= _imageIn->width() && 0 <= it->y0 && it->y0 + it->height <= _imageIn->height());

    _imageOut = (VideoRGBAType*)*vit;
    int x = it->x0;
    int width = (int)it->width;

    for (int y=0; y<(int)it->height; ++y)
      memcpy(_imageOut->row(y), &_imageIn->operator()(x, y), width);
  }
}
