/* Gear_VideoSwitch.cpp
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

#include "Gear_VideoSwitch.h"                       
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoSwitch, Gear_VideoSwitch, "VideoSwitch")

Gear_VideoSwitch::Gear_VideoSwitch(Engine *engine, std::string name) : Gear(engine, "VideoSwitch", name)
{
  addPlug(_VIDEO_IN_A = new PlugIn<VideoRGBAType>(this, "ImgA"));
  addPlug(_VIDEO_IN_B = new PlugIn<VideoRGBAType>(this, "ImgB"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgO"));

  EnumType *switchIn = new EnumType(2, 0);
  switchIn->setLabel(0, "Play A");
  switchIn->setLabel(1, "Play B");
  
  addPlug(_SWITCH_IN = new PlugIn<EnumType>(this, "Switch", switchIn));
}

Gear_VideoSwitch::~Gear_VideoSwitch()
{
}

bool Gear_VideoSwitch::ready()
{
  return (((_VIDEO_IN_A->connected() && _SWITCH_IN->type()->value()==0) ||
           (_VIDEO_IN_B->connected() && _SWITCH_IN->type()->value()==1)) &&
          _VIDEO_OUT->connected());
}

void Gear_VideoSwitch::runVideo()
{
  _playA = (_SWITCH_IN->type()->value() == 0);

  if (_playA)
    _image = _VIDEO_IN_A->type();
  else
    _image = _VIDEO_IN_B->type();

  if (_image->isNull())
    return;
  
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  memcpy(_outImage->data(), _image->data(), _image->size()*sizeof(RGBA));

}
