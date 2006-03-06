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

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
	return new Gear_VideoSwitch(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VideoSwitch";
  gearInfo.classification = GearClassifications::video().instance();
  return gearInfo;
}
}

Gear_VideoSwitch::Gear_VideoSwitch(Schema *schema, std::string uniqueName) : Gear(schema, "VideoSwitch", uniqueName)
{
  addPlug(_VIDEO_OUT_A = new PlugOut<VideoRGBAType>(this, "ImgA", false));
  addPlug(_VIDEO_OUT_B = new PlugOut<VideoRGBAType>(this, "ImgB", false));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIn", true));

  EnumType *switchIn = new EnumType(2, 0);
  switchIn->setLabel(0, "Play A");
  switchIn->setLabel(1, "Play B");
  
  addPlug(_SWITCH_IN = new PlugIn<EnumType>(this, "Switch", false, switchIn));
}

Gear_VideoSwitch::~Gear_VideoSwitch()
{
}

void Gear_VideoSwitch::runVideo()
{
  _playA = (_SWITCH_IN->type()->value() == 0);
	
	if((!_VIDEO_OUT_A->connected() && _playA) ||
		 (!_VIDEO_OUT_B->connected() && !_playA))
	{
		_VIDEO_OUT_A->sleeping(true);
		_VIDEO_OUT_B->sleeping(true);
		return;
	}
	
  if (_playA)
	{  
		_outImage = _VIDEO_OUT_A->type();
		_VIDEO_OUT_A->sleeping(false);
		_VIDEO_OUT_B->sleeping(true);
  }
	else
	{	
		_outImage = _VIDEO_OUT_B->type();
		_VIDEO_OUT_A->sleeping(true);
		_VIDEO_OUT_B->sleeping(false);
	}	
	
	_image = _VIDEO_IN->type();
	
  if (_image->isNull())
    return;
  
  _outImage->resize(_image->width(), _image->height());
	
  memcpy(_outImage->data(), _image->data(), _image->size()*sizeof(RGBA));

}
