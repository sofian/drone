/* Gear_MotionTrack.h
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

#ifndef GEAR_VIDEOBLEND_INCLUDED
#define GEAR_VIDEOBLEND_INCLUDED

#include <opencv/cv.hpp>

#include "Gear.h"
#include "ValueType.h"
#include "VideoRGBAType.h"
#include "Rasterer.h"

#define IMG_BUFFER_SIZE 3

class Gear_MotionTrack : public Gear
{
public:

  Gear_MotionTrack(Engine *engine, std::string name);
  virtual ~Gear_MotionTrack();

  void init();
  void runVideo();
  bool ready();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugIn<ValueType> *_THRESHOLD;
  PlugIn<ValueType> *_HISTOGRAM_BINS;
  PlugIn<ValueType> *_MIN_S;
  PlugIn<ValueType> *_MAX_S;
  PlugIn<ValueType> *_MIN_V;
  PlugIn<ValueType> *_MAX_V;
  
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  //local var
  const VideoRGBAType *_image;
  VideoRGBAType *_outImage;

  CvCamShiftTracker *_tracker;
  IplImage *_image_rgba;
  IplImage *_image_rgb;
  
  int _sizeX, _sizeY;

  bool _firstRun;
};

#endif
