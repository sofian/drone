/* Gear_TA_MapNavigator.h
 * Copyright (C) 2005 Jean-Sebastien Senecal
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

#ifndef GEAR_TA_MAPInfoLayer_INCLUDED
#define GEAR_TA_MAPInfoLayer_INCLUDED


#include "Gear.h"
#include "ValueType.h"
#include "TA_DataType.h"
#include "VideoRGBAType.h"
#include "VectorialType.h"

class Gear_TA_MapInfoLayer : public Gear
{
public:  

  Gear_TA_MapInfoLayer(Schema *schema, std::string uniqueName);
  virtual ~Gear_TA_MapInfoLayer();

  void runVideo();

public:

  PlugIn<TA_DataType> *_DATA_IN;
  PlugIn<VectorialType> *_VEC_IN;
  PlugIn<ValueType> *_RAX;
  PlugIn<ValueType> *_RAY;
  PlugIn<ValueType> *_XOFF;
  PlugIn<ValueType> *_YOFF;
  PlugIn<ValueType> *_SCALE;
  
  PlugIn<ValueType> *_RADIUS;
  PlugIn<ValueType> *_MAGN;
  
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<VectorialType> *_VEC_OUT;
  
private: 
  VideoRGBAType* _image;
  VideoRGBAType* _outImage;
  unsigned int* _data;
  unsigned int* _outData;
};

#endif
