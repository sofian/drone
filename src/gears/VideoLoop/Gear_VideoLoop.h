/* Gear_VideoLoop.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef GEAR_VIDEOLOOP_INCLUDED
#define GEAR_VIDEOLOOP_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"
#include "EnumType.h"

template <class T>
class CircularBuffer;

class Gear_VideoLoop : public Gear
{
public:
  enum ePlaybackMode
  {
    FORWARD,
    BACKWARD,
    PING_PONG,
    N_PLAYBACK_MODE
  };

  Gear_VideoLoop(Schema *schema, std::string uniqueName);
  virtual ~Gear_VideoLoop();

  void runVideo();

  bool ready();

protected:
  void internalInit();    

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_PUNCH_OUT;
  PlugIn<ValueType> *_PUNCH_IN;
  PlugIn<ValueType> *_MEMORY;
  PlugIn<EnumType> *_MODE_IN;

  //local var
  const VideoRGBAType *_image;     
  VideoRGBAType *_outImage; 
  unsigned char *_data;

  unsigned char *_outData;

  int _sizeY;
  int _sizeX;

  CircularBuffer<RGBA> * _circbuf;

  bool _recording;
  int _currentLoopFrame;
  int _nLoopFrames;
  int _memory;
  int _playbackMode;
  int _pingpongDir;
  
};

#endif
