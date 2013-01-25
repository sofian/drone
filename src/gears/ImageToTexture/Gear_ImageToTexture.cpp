/* Gear_ImageToTexture.cpp
 * Copyright (C) 2012 Mathieu Guindon
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

#include "Gear_ImageToTexture.h"                       
#include "Engine.h"
#include "DroneMath.h"

#include <iostream>


extern "C" {
Gear* makeGear()
{
  return new Gear_ImageToTexture();
}

}

Gear_ImageToTexture::Gear_ImageToTexture()
: Gear("ImageToTexture")
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_TEXTURE_OUT = new PlugOut<TextureType>(this, "TexOUT", true));
}

Gear_ImageToTexture::~Gear_ImageToTexture()
{

}


bool Gear_ImageToTexture::ready()
{
  return(_VIDEO_IN->connected() && _TEXTURE_OUT->connected());
}

void Gear_ImageToTexture::runVideo()
{
  if (_VIDEO_IN->type()->isNull())
    return;
  
  const VideoRGBAType *image = _VIDEO_IN->type();
  TextureType *texture = _TEXTURE_OUT->type();
  
  texture->updateWithVideoRGBA(*image);
}

