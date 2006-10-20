/* Gear_DummySource.h
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

#ifndef GEAR_VIDEOSOURCE_INCLUDED
#define GEAR_VIDEOSOURCE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "ValueType.h"
#include "Rasterer.h"

#include <libmpeg3.h>

class Gear_DummySource : public Gear
{
public:
  Gear_DummySource(Schema *schema, std::string uniqueName);
  virtual ~Gear_DummySource();

  void runVideo();
  void runAudio();

  bool ready();

protected:
  void onUpdateSettings();

private:
  
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_X_IN;
  PlugIn<ValueType> *_Y_IN;
  PlugIn<ValueType> *_INTENSITY_IN;
  
  VideoRGBAType *_outImage;
  Rasterer *_rast;

  //locals

};

#endif
