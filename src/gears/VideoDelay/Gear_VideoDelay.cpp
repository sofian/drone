/* Gear_VideoDelay.cpp
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

#include "Gear_VideoDelay.h"
#include "Engine.h"
#include "CircularBuffer.h"

#include <iostream>

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, QString uniqueName)
{
  return new Gear_VideoDelay(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VideoDelay";
  gearInfo.classification = GearClassifications::video().time().instance();
  return gearInfo;
}
}

Gear_VideoDelay::Gear_VideoDelay(Schema *schema, QString uniqueName) : Gear(schema, "VideoDelay", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  addPlug(_MEMORY = new PlugIn<ValueType>(this, "Memory", false, new ValueType(125, 0, 125)));
  addPlug(_DELAY = new PlugIn<ValueType>(this, "Delay", false, new ValueType(0,-124,0)));
  addPlug(_RECORD = new PlugIn<ValueType>(this, "Rec", false, new ValueType(1,0,1)));

  circbuf = new CircularBuffer<RGBA>(BLACK_RGBA);

}

Gear_VideoDelay::~Gear_VideoDelay()
{
}

void Gear_VideoDelay::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;


  _sizeY = _image->height();
  _sizeX = _image->width();

  _delay = CLAMP(_DELAY->type()->intValue(),-_memory+1,0);
  _memory=_MEMORY->type()->intValue();

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_sizeX, _sizeY);
 
  circbuf->resize(_sizeY*_sizeX, _memory);
  if(_RECORD->type()->intValue())
    circbuf->append(_image->data());

  circbuf->fillVectorFromBlock(_outImage, _delay);
  
}
