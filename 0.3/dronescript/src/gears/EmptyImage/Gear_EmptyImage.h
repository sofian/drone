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

#ifndef GEAR_EMPTYIMAGE_INCLUDED
#define GEAR_EMPTYIMAGE_INCLUDED

#include "Gear.h"
#include "VideoRGBAType.h"
#include "ValueType.h"

class Gear_EmptyImage : public Gear
{
public:

  Gear_EmptyImage(Schema *schema, std::string name);
  virtual ~Gear_EmptyImage();

  void runVideo();


private:
  // Inputs.
  PlugIn<ValueType> *_XSIZE;  
  PlugIn<ValueType> *_YSIZE;

  // Outputs.
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  Array2D<RGBA> _current;

};

#endif
