/* Gear_ColorAdjust.h
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

#ifndef GEAR_COLORADJUST_INCLUDED
#define GEAR_COLORADJUST_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"


class Gear_ColorAdjust : public Gear
{
public:

  Gear_ColorAdjust(Engine *engine, std::string name);
  virtual ~Gear_ColorAdjust();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_RED_IN;
  PlugIn<ValueType> *_GREEN_IN;
  PlugIn<ValueType> *_BLUE_IN;

  //local var
  const VideoTypeRGBA *_image; 
  VideoTypeRGBA *_outImage; 

  int _size;

  int *_mmxImageIn;
  int *_mmxImageOut;    
  unsigned long long int _mmxColor;

  short _r;
  short _g;
  short _b;

  unsigned char *_imageIn;
  unsigned char *_imageOut;

};

#endif

