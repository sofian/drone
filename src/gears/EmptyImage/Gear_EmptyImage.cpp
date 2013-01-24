/* Gear_ImageSource.cpp
 * Copyright (C) 2004--2005 Jean-Sebastien Senecal
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
#include <stdio.h>

#include "Gear_EmptyImage.h"
#include "Engine.h"


#include "DroneMath.h"

extern "C" {
Gear* makeGear()
{
  return new Gear_EmptyImage();
}


}

Gear_EmptyImage::Gear_EmptyImage() : 
  Gear("EmptyImage")
{
  // Inputs.
  addPlug(_XSIZE = new PlugIn<ValueType>(this, "xsize", false, new ValueType(320, 0, 720)));
  addPlug(_YSIZE = new PlugIn<ValueType>(this, "ysize", false, new ValueType(240, 0, 480)));

  // Outputs.
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));


}

Gear_EmptyImage::~Gear_EmptyImage()
{
}

void Gear_EmptyImage::runVideo()
{

  int xs = _XSIZE->type()->value();
  int ys = _YSIZE->type()->value();
  
  if(xs!=_VIDEO_OUT->type()->width() || ys!=_VIDEO_OUT->type()->height())
    {
      _VIDEO_OUT->type()->resize(xs,ys);
      _VIDEO_OUT->type()->fill(WHITE_RGBA);
    }
}



