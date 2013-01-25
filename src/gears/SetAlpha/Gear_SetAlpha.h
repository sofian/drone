/* Gear_SetAlpha.h
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

#ifndef GEAR_SETALPHA_INCLUDED
#define GEAR_SETALPHA_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "VideoChannelType.h"


class Gear_SetAlpha : public Gear
{
public:

  Gear_SetAlpha();
  virtual ~Gear_SetAlpha();

  void runVideo();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugIn<VideoChannelType> *_ALPHA_MASK_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  
  //local var
  const VideoRGBAType *_image;
  const VideoChannelType *_mask; 
  VideoRGBAType *_outImage; 

};

#endif
