/* Gear_VideoLoop.cpp
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

#include "Gear_VideoLoop.h"
#include "Engine.h"
#include "CircularBuffer.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoLoop, Gear_VideoLoop, "VideoLoop")

Gear_VideoLoop::Gear_VideoLoop(Engine *engine, std::string name) : Gear(engine, "VideoLoop", name),
                                                                     _recording(true),
                                                                     _currentLoopFrame(0),
                                                                     _nLoopFrames(1),
                                                                     _pingpongDir(1)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  addPlug(_PUNCH_IN = new PlugIn<ValueType>(this, "PunchIn", new ValueType(0, 0, 1)));
  addPlug(_PUNCH_OUT = new PlugIn<ValueType>(this, "PunchOut", new ValueType(0, 0, 1)));
  addPlug(_MEMORY = new PlugIn<ValueType>(this, "Memory", new ValueType(125, 0, 125)));

  EnumType *playbackMode = new EnumType(N_PLAYBACK_MODE, FOWARD);
  playbackMode->setLabel(FOWARD,"Foward");
  playbackMode->setLabel(BACKWARD,"Backward");
  playbackMode->setLabel(PING_PONG,"Ping pong");
  addPlug(_MODE_IN = new PlugIn<EnumType>(this, "Mode", playbackMode));

  _circbuf = new CircularBuffer<RGBA>(BLACK_RGBA);

}

Gear_VideoLoop::~Gear_VideoLoop()
{
}

void Gear_VideoLoop::init()
{
  _recording = true;
  _currentLoopFrame = 0;
  _nLoopFrames = 0;
}

bool Gear_VideoLoop::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_VideoLoop::runVideo()
{
  _image = _VIDEO_IN->type();
  
  _sizeY = _image->height();
  _sizeX = _image->width();

  _outImage = _VIDEO_OUT->type();

  _outImage->resize(_sizeX, _sizeY);

  _playbackMode = CLAMP((ePlaybackMode)_MODE_IN->type()->value(), FOWARD, PING_PONG);

  _memory = MAX(_MEMORY->type()->intValue(), 0);
  _circbuf->resize(_sizeY*_sizeX, _memory);

  // Punch in!
  if ((int)_PUNCH_IN->type()->value() == 1)
  {
    NOTICE("Punch in");
    // Start recording.
    _recording = true;
    _nLoopFrames = 0;
  }
  
  if (_recording)
  {
    // Now recording.
    _circbuf->append(_image->data()); // append current image
    _nLoopFrames = MIN(_nLoopFrames+1,_memory); // update number of frames
    
    // Punch out!
    if ((int)_PUNCH_OUT->type()->value() == 1)
    {
      NOTICE("Punch out");
      // Stop recording.
      _recording = false;
      _currentLoopFrame = 0;
    }
    
    _circbuf->fillVectorFromBlock(_outImage, 0);
  }
  else
  {
    ASSERT_ERROR (_nLoopFrames > 0);
    
    switch (_playbackMode)
    {
    case FOWARD:
        _currentLoopFrame = (_currentLoopFrame + 1) % _nLoopFrames;
      break;
    case BACKWARD:
        if (_currentLoopFrame < 0)
           _currentLoopFrame = _nLoopFrames-1;
         _currentLoopFrame--;
      break;
    case PING_PONG:
        if (_currentLoopFrame < 0)
          _pingpongDir=1;
        else if (_currentLoopFrame >= _nLoopFrames)
          _pingpongDir=-1;
        _currentLoopFrame+=_pingpongDir;
      break;
    }

    
    _circbuf->fillVectorFromBlock(_outImage, _currentLoopFrame -_nLoopFrames + 1);
  }
 
  
}
