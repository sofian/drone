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
//inspired from Using libavformat and libavcodec by Martin Böhme (boehme@inb.uni-luebeckREMOVETHIS.de) 


#include <iostream>
#include "Gear_SelectFrame.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_SelectFrame(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "SelectFrame";
    gearInfo.classification = GearClassifications::video().IO().instance();
    return gearInfo;
  }
}

Gear_SelectFrame::Gear_SelectFrame(Schema *schema, std::string uniqueName) : 
  Gear(schema, "SelectFrame", uniqueName)
{
  // Inputs.
  addPlug(_NEXT_IN     = new PlugIn<ValueType>(this, "Next",    new ValueType(1, 0, 1)));
  //  addPlug(_RESET_IN    = new PlugIn<ValueType>(this, "Reset",   new ValueType(0, 0, 1)));
  addPlug(_N_FRAMES_IN = new PlugIn<ValueType>(this, "NFrames", new ValueType(0, 0, 0)));
  addPlug(_SEEK_IN     = new PlugIn<ValueType>(this, "Seek",    new ValueType(0, -125, 125)));

  EnumType *playbackMode = new EnumType(N_PLAYBACK_MODE, FORWARD);
  playbackMode->setLabel(FORWARD,"Foward");
  playbackMode->setLabel(BACKWARD,"Backward");
  playbackMode->setLabel(PING_PONG,"Ping pong");
  playbackMode->setLabel(RANDOM,"Random");
  addPlug(_MODE_IN     = new PlugIn<EnumType>(this, "Mode",     playbackMode));

  // Outputs.
  addPlug(_FRAME_OUT   = new PlugOut<ValueType>(this, "Frame",  new ValueType(0, 0, 0)));
}

Gear_SelectFrame::~Gear_SelectFrame()
{
}

void Gear_SelectFrame::init()
{
  _currentFrame = 0;
  _nFrames = 0;
}

bool Gear_SelectFrame::ready()
{
  return(_N_FRAMES_IN->connected() || _FRAME_OUT->connected());
}

void Gear_SelectFrame::runVideo()
{
  _nFrames = MAX( _N_FRAMES_IN->type()->intValue(), 0 );
  
  // Playback.
  if (_nFrames > 0)
  {
    if (_NEXT_IN->type()->boolValue())
    {
      // Step.
      switch ((ePlaybackMode)_MODE_IN->type()->value())
      {
      case BACKWARD:
        _currentSeekFrame = REPEAT_CLAMP(_currentFrame + _SEEK_IN->type()->intValue(), 0, _nFrames-1);
        _currentFrame = (_currentFrame==0? _nFrames-1 : _currentFrame-1);
        break;
      case PING_PONG:
        _currentSeekFrame = MIRROR_CLAMP(_currentFrame + _SEEK_IN->type()->intValue(), 0, _nFrames-1);
        _currentFrame = (_currentFrame + 1 % _nFrames);
        break;
      case RANDOM:
        // For the moment the reset and seek buttons do nothing with random.
        _currentSeekFrame = _currentFrame = rand() % _nFrames;
        break;
      case FORWARD:
      default:
        _currentSeekFrame = REPEAT_CLAMP(_currentFrame + _SEEK_IN->type()->intValue(), 0, _nFrames-1);
        _currentFrame = (_currentFrame + 1 % _nFrames);
        break;
      }
    }

    // Set seek frame to output.
    _FRAME_OUT->type()->setValue(_currentSeekFrame);
  }
  else // default: send dummy frame
    _FRAME_OUT->type()->setValue(-1);
}



