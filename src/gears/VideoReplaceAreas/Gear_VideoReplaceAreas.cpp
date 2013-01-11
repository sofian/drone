/* Gear_VideoReplaceAreas.cpp
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
#include "Gear_VideoReplaceAreas.h"
#include "Engine.h"

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, QString uniqueName)
{
  return new Gear_VideoReplaceAreas(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VideoReplaceAreas";
  gearInfo.classification = GearClassifications::video().mask().instance();
  return gearInfo;
}
}

Gear_VideoReplaceAreas::Gear_VideoReplaceAreas(Schema *schema, QString uniqueName) : Gear(schema, "VideoReplaceAreas", uniqueName)
{
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "VideoOut", true));
  
  addPlug(_VIDEO_LIST_IN = new PlugIn<ListType>(this, "VideoList", true));
  addPlug(_REPLACE_IN = new PlugIn<AreaArrayType>(this, "Replace", true));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "VideoIn", true));
}

Gear_VideoReplaceAreas::~Gear_VideoReplaceAreas()
{
}

void Gear_VideoReplaceAreas::runVideo()
{
  _areaArray = _REPLACE_IN->type();
  _imageIn = _VIDEO_IN->type();
  _listIn = _VIDEO_LIST_IN->type();

  _imageOut = _VIDEO_OUT->type();
  
  ASSERT_ERROR(_listIn->size() == _areaArray->size());
  
  ListType::const_iterator vit = _listIn->begin();
  for (AreaArrayType::const_iterator it = _areaArray->begin(); it != _areaArray->end(); ++it, ++vit)
  {
    ASSERT_ERROR(0 <= it->x0 && it->x0 + it->width <= _imageIn->width() && 0 <= it->y0 && it->y0 + it->height <= _imageIn->height());
    _subImageIn = (VideoRGBAType*)*vit;

    ASSERT_ERROR(it->width == _imageOut->width() && it->height == _imageOut->height());

    int x = it->x0;
    int width = (int)it->width;
    
    for (int y=0; y<(int)it->height; ++y)
      memcpy(&_imageOut->operator()(x, y), _subImageIn->row(y), width);
  }
}


