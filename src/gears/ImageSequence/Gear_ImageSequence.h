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

#ifndef GEAR_IMAGESEQUENCE_INCLUDED
#define GEAR_IMAGESEQUENCE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "EnumType.h"
#include "ValueType.h"

#include <qimage.h>

class Gear_ImageSequence : public Gear
{

  enum eOrderingType 
  {
    SHUFFLE,         // 0 
    SEQUENCE,
    N_ORDERING_TYPES
  };

public:
  static const std::string SETTING_FILENAME;

  Gear_ImageSequence(Schema *schema, std::string name);
  virtual ~Gear_ImageSequence();

  void init();
  
  void runVideo();

  bool ready();

  void onUpdateSettings();

private:
  void loadImage(const std::string& filename, Array2D<RGBA>& image);
  
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_SPEED_IN;
  PlugIn<EnumType> *_ORDERING_IN;
 
  Array< Array2D<RGBA> > _imageBuffers;

  int _current;
  int _nFramesCurrent;

  eOrderingType _orderingType;

};

#endif
