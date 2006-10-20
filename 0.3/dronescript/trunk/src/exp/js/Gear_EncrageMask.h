/* Gear_EncrageMask.h
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

#ifndef GEAR_ENCRAGEMASK_INCLUDED
#define GEAR_ENCRAGEMASK_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "AreaArrayType.h"
#include <stdlib.h>
//#include "ValueType.h"
#include "error.h"

class Gear_EncrageMask : public Gear
{
public:
  Gear_EncrageMask(Schema *schema, std::string uniqueName);
  virtual ~Gear_EncrageMask();

  void runVideo();

  bool ready();

private:
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<VideoRGBAType> *_VIDEO_INK_IN;
  PlugIn<VideoRGBAType> *_VIDEO_DECAY_IN;
  
  // Ink speeds is in % covered in each area / frame
  PlugIn<ValueType> *_INK_SPEED_IN;
  // Decay speed is in % decaying everywhere / frame
  PlugIn<ValueType> *_DECAY_SPEED_IN;
  //  PlugIn<ValueType> *_OPACITY_IN;
  PlugIn<AreaArrayType> *_AREA_ARRAY_IN;

  //int _sizeX, _sizeY;

  const VideoRGBAType *_imageInk;
  const VideoRGBAType *_imageDecay;
  
  VideoRGBAType *_imageOut;
  VideoRGBAType *_imageBuffer;
  const AreaArrayType *_areaArray;
  int _sizeX, _sizeY, _size;
  float _inkSpeed, _decaySpeed;
};

#endif
