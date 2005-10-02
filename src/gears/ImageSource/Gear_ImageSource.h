/* Gear_ImageSource.h
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

#ifndef GEAR_IMAGESOURCE_INCLUDED
#define GEAR_IMAGESOURCE_INCLUDED

#include "Gear.h"
#include "VideoRGBAType.h"
#include "ValueType.h"

#include <qimage.h>

class Gear_ImageSource : public Gear
{
public:
  static const std::string SETTING_FILENAME;

  Gear_ImageSource(Schema *schema, std::string name);
  virtual ~Gear_ImageSource();

  void runVideo();

  void onUpdateSettings();

private:
  // Inputs.
  PlugIn<ValueType> *_FRAME_IN;

  // Outputs.
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<ValueType> *_N_FRAMES_OUT;

  // Functions.
  void loadImage(const std::string& filename, Array2D<RGBA>& image);

  // local
  Array< Array2D<RGBA> > _imageBuffers;
  Array2D<RGBA> *_current;

};

#endif
