/* Gear_SelectFrame.cpp
 * Copyright (C) 2004--2005 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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
//inspired from Using libavformat and libavcodec by Martin B�hme (boehme@inb.uni-luebeckREMOVETHIS.de) 


#include <iostream>
#include "Gear_SelectFrame.h"
#include "Engine.h"



extern "C" {           
  Gear* makeGear()
  {
    return new Gear_SelectFrame();
  }  
  
}

Gear_SelectFrame::Gear_SelectFrame() : 
  Gear("SelectFrame")
{
  // Inputs.
  addPlug(_NEXT_IN = new PlugIn<ValueType>(this, "Next", false, new ValueType(1, 0, 1)));
  //  addPlug(_RESET_IN    = new PlugIn<ValueType>(this, "Reset",   new ValueType(0, 0, 1)));
  addPlug(_N_FRAMES_IN = new PlugIn<ValueType>(this, "NFrames", false, new ValueType(0, 0, 0)));
  addPlug(_SEEK_IN = new PlugIn<ValueType>(this, "Seek", false, new ValueType(0, -125, 125)));
  addPlug(_RESET_IN = new PlugIn<ValueType>(this, "Reset", false, new ValueType(0, 0, 1)));


  EnumType *playbackMode = new EnumType(N_PLAYBACK_MODE, FORWARD);
  playbackMode->setLabel(LOOPFORWARD,"LoopFoward");
  playbackMode->setLabel(LOOPBACKWARD,"LoopBackward");
  playbackMode->setLabel(FORWARD,"Foward");
  playbackMode->setLabel(BACKWARD,"Backward");
  playbackMode->setLabel(PING_PONG,"Ping pong");
  playbackMode->setLabel(RANDOM,"Random");
  addPlug(_MODE_IN = new PlugIn<EnumType>(this, "Mode", playbackMode));

  // Outputs.
  addPlug(_FRAME_OUT = new PlugOut<ValueType>(this, "Frame",  true, new ValueType(0, 0, 0)));
}

Gear_SelectFrame::~Gear_SelectFrame()
{
}

void Gear_SelectFrame::internalInit()
{
  _currentFrame = 0;
  _nFrames = 0;
}


void Gear_SelectFrame::runVideo()
{
    _nFrames = MAX( _N_FRAMES_IN->type()->intValue(), 0 );
  // Playback.
  if (_nFrames > 0)
  {
    bool next = _NEXT_IN->type()->boolValue();
    bool reset =_RESET_IN->type()->boolValue();
       
    // Step.
    switch ((ePlaybackMode)_MODE_IN->type()->value())
    {
    case PING_PONG:
      _currentSeekFrame = MIRROR_CLAMP(_currentFrame + _SEEK_IN->type()->intValue(), 0, _nFrames-1);
      if(reset)
        _currentFrame = 0;
      else if (next)
      _currentFrame = (_currentFrame + 1 % _nFrames);
      break;
    case RANDOM:
      // For the moment the reset and seek buttons do nothing with random.
      _currentSeekFrame = rand() % _nFrames;
      if(reset)
        _currentFrame = 0;
      else if (next)
        _currentFrame = rand() % _nFrames;
      break;
    
    case BACKWARD:
      _currentSeekFrame = CLAMP(_currentFrame + _SEEK_IN->type()->intValue(), 0, _nFrames-1);
      if(reset)
        _currentFrame = _nFrames-1;
      else if (next)
        _currentFrame = CLAMP(_currentFrame-1,-1,_nFrames-1);
      // if video is finished, set _currentSeekFrame to -1
      if(_currentFrame==-1)
        _currentSeekFrame=-1;
      break;
    case FORWARD:
      _currentSeekFrame = CLAMP(_currentFrame + _SEEK_IN->type()->intValue(), 0, _nFrames-1);

      if(reset)
        _currentFrame = 0;
      else if (next)
        _currentFrame = CLAMP(_currentFrame+1,0,_nFrames);
      
      
      // if video is finished, set _currentSeekFrame to -1
      if(_currentFrame==_nFrames)
        _currentSeekFrame=-1;
      break;

    case LOOPBACKWARD:
      _currentSeekFrame = REPEAT_CLAMP(_currentFrame + _SEEK_IN->type()->intValue(), 0, _nFrames-1);
      if(reset)
        _currentFrame = _nFrames-1;
      else if (next)
        _currentFrame = (_currentFrame==0? _nFrames-1 : _currentFrame-1);
      break;
    case LOOPFORWARD:
    default:
      _currentSeekFrame = REPEAT_CLAMP(_currentFrame + _SEEK_IN->type()->intValue(), 0, _nFrames-1);
      if(reset)
        _currentFrame = 0;
      else if (next)
        _currentFrame = (_currentFrame + 1 % _nFrames);
      break;
    }
  
    // Set seek frame to output.
    _FRAME_OUT->type()->setValue(_currentSeekFrame);
  }
  else // default: send dummy frame
    _FRAME_OUT->type()->setValue(-1);
}



