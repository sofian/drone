/* Gear_VideoDelay.h
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

#ifndef GEAR_VIDEODELAY_INCLUDED
#define GEAR_VIDEODELAY_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"

template <class T>
class CircularBuffer;

class Gear_VideoDelay : public Gear
{
public:

  Gear_VideoDelay(Schema *schema, QString uniqueName);
  virtual ~Gear_VideoDelay();

  void runVideo();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_MEMORY;
  PlugIn<ValueType> *_DELAY;
  PlugIn<ValueType> *_RECORD;

  //local var
  const VideoRGBAType *_image;     
  VideoRGBAType *_outImage; 
  unsigned char *_data;

  unsigned char *_outData;

  int _sizeY;
  int _sizeX;
  int _delay;
  int _memory;

  CircularBuffer<RGBA> * circbuf;  
};

#endif
