/* Gear_ImageShuffle.h
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

#ifndef GEAR_IMAGESHUFFLE_INCLUDED
#define GEAR_IMAGESHUFFLE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "ValueType.h"

#include <qimage.h>

class Gear_ImageShuffle : public Gear
{
public:
  static const std::string SETTING_FILENAME;

  Gear_ImageShuffle(Schema *schema, std::string name);
  virtual ~Gear_ImageShuffle();

  void init();
  
  void runVideo();

  bool ready();

  void onUpdateSettings();

private:
  void loadImage(const std::string& filename, Array2D<RGBA>& image);
  
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_SPEED_IN;
  
  Array< Array2D<RGBA> > _imageBuffers;

  int _current;
  int _nFramesCurrent;
};

#endif
