/* Gear_Brightness.h
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#ifndef GEAR_BRIGHTNESS_INCLUDED
#define GEAR_BRIGHTNESS_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"


class Gear_Brightness : public Gear
{
public:

  Gear_Brightness(Engine *engine, std::string name);
  virtual ~Gear_Brightness();

  void runVideo();  bool ready();

private:


  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_BRIGHTNESS_IN;

  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 

  // Lookup table for brightness values.
  unsigned char _lut[256];

  int _size;

  const unsigned char *_imageIn;
  unsigned char *_imageOut;

};

#endif
