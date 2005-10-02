/* Gear_Rescale.h
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

#ifndef GEAR_RESCALE_INCLUDED
#define GEAR_RESCALE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "ValueType.h"
#include "Utils.h"

class Gear_Rescale : public Gear
{
public:

  Gear_Rescale(Schema *schema, std::string uniqueName);
  virtual ~Gear_Rescale();

  void runVideo();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_WIDTH_IN;
  PlugIn<ValueType> *_HEIGHT_IN;

  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 

  int _newWidth;
  int _newHeight;
};

#endif
