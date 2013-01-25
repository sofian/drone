/* VideoChannelType.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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
#include "VideoChannelType.h"

#ifndef GEAR_IMAGE_TO_TEXTURE_INCLUDED
#define GEAR_IMAGE_TO_TEXTURE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
 #include "TextureType.h"


class Gear_ImageToTexture : public Gear
{
public:

  Gear_ImageToTexture();
  virtual ~Gear_ImageToTexture();

  void runVideo();
  virtual bool ready();
private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<TextureType> *_TEXTURE_OUT;
};

#endif
