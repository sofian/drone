/* Gear_Oscilloscope.h
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

#ifndef GEAR_OSCILLOSCOPE_INCLUDED
#define GEAR_OSCILLOSCOPE_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"

template<class T>
class CircularBuffer;

class PlugVideo;
class PlugSignal;

class Gear_Oscilloscope : public Gear
{
public:

  Gear_Oscilloscope(Schema *schema, std::string uniqueName);
  virtual ~Gear_Oscilloscope();

  void runAudio();
  void runVideo();

  bool ready();

private:

  PlugIn<SignalType> *_AUDIO_IN;
  PlugIn<ValueType> *_ZOOM_X;
  PlugIn<ValueType> *_ZOOM_Y;
  PlugIn<ValueType> *_SIZE_X;
  PlugIn<ValueType> *_SIZE_Y;
  
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  VideoRGBAType *_outImage; 
  RGBA *_outData;

  CircularBuffer<Signal_T> * circbuf;
  
  // contains average of samples over 1 pixel
  // must be recomputed every time zoomx or sizex is changed
  CircularBuffer<Signal_T> * pixbuf;
};

#endif
