/* Gear_VideoSwitch.h
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

#ifndef GEAR_VIDEOSWITCH_INCLUDED
#define GEAR_VIDEOSWITCH_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "ValueType.h"
#include "EnumType.h"
#include "VideoRGBAType.h"
#include "Utils.h"

class Gear_VideoSwitch : public Gear
{
public:

  Gear_VideoSwitch();
  virtual ~Gear_VideoSwitch();

  void runVideo();
  
private:

  PlugOut<VideoRGBAType> *_VIDEO_OUT_A;
  PlugOut<VideoRGBAType> *_VIDEO_OUT_B;
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugIn<EnumType> *_SWITCH_IN;

  //local var
  const VideoRGBAType *_image;
  VideoRGBAType *_outImage; 

  bool _playA;

};

#endif
