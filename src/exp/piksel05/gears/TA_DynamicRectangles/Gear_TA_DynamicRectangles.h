/* Gear_TA_DynamicRectangles.h
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

#ifndef GEAR_TA_DYNAMICRECTANGLES_INCLUDED
#define GEAR_TA_DYNAMICRECTANGLES_INCLUDED


#include "Gear.h"
#include "ValueType.h"
#include "VideoRGBAType.h"
#include "AreaType.h"
#include "TA_DataType.h"
//#include "TA_Effects.h"

#include <list>

struct Rectangle
{
  AreaType area;
  VideoRGBAType img;
  float blend;

  unsigned char _lut[256];

  void init(float gamma);
  void clipImage(const VideoRGBAType& imageIn);
  void applyGamma();
  void recomputeAlpha();
  void run(VideoRGBAType& image) const;
};

typedef std::list< Rectangle > RectangleList;

class Gear_TA_DynamicRectangles : public Gear
{
public:

  Gear_TA_DynamicRectangles(Schema *schema, std::string uniqueName);
  virtual ~Gear_TA_DynamicRectangles();

  void runVideo();

public:
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugIn<ValueType> *_DECAY_IN;
  PlugIn<ValueType> *_WIDTH_IN;
  PlugIn<ValueType> *_HEIGHT_IN;
//   PlugIn<ValueType> *_INNOCENCE_IN;
//   PlugIn<ValueType> *_CHANNEL_IN;

  PlugIn<ValueType> *_GAMMA_IN;
  PlugIn<ValueType> *_NEW_IN;


  PlugOut<VideoRGBAType> *_VIDEO_OUT;

protected:
  const VideoRGBAType *_imageIn;
  VideoRGBAType *_imageInBuffer;
  VideoRGBAType *_imageOut;

  RectangleList _rectangles;
  
  void createRectangle(size_t width, size_t height, float gamma);
  //  void alphaBlend(const Rectangle& rect, VideoRGBAType& imageIn);
};

#endif
