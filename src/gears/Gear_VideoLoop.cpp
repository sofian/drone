/* Gear_VideoLoop.cpp
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#include "Gear_VideoLoop.h"
#include "Engine.h"
#include "CircularBuffer.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoLoop, Gear_VideoLoop, "VideoLoop")

Gear_VideoLoop::Gear_VideoLoop(Engine *engine, std::string name) : Gear(engine, "VideoLoop", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_PUNCH_IN = new PlugIn<ValueType>(this, "PunchIn", new ValueType(0, 0, 1)));
  addPlug(_PUNCH_OUT = new PlugIn<ValueType>(this, "PunchOut", new ValueType(0, 0, 1)));
  addPlug(_MEMORY = new PlugIn<ValueType>(this, "Memory", new ValueType(125, 0, 125)));

  circbuf = new CircularBuffer<RGBA>(BLACK_RGBA);

}

Gear_VideoLoop::~Gear_VideoLoop()
{
}

void Gear_VideoLoop::init()
{
}

bool Gear_VideoLoop::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_VideoLoop::runVideo()
{
  _image = _VIDEO_IN->type();

  _sizeY = _image->height();
  _sizeX = _image->width();

  _outImage = _VIDEO_OUT->type();

  _outImage->resize(_sizeX, _sizeY);
 
  circbuf->resize(_sizeY*_sizeX, _MEMORY->type()->intValue());
  circbuf->append(_image->data());

  circbuf->fillVectorFromBlock(_outImage, - (int)rand() % 4);  
}
