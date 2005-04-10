/* Gear_VideoRecord.cpp
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

#include "Gear_VideoRecord.h"
#include "Engine.h"
#include "CircularBuffer.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_VideoRecord(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VideoRecord";
  gearInfo.classification = GearClassifications::video().time().instance();
  return gearInfo;
}
}

Gear_VideoRecord::Gear_VideoRecord(Schema *schema, std::string uniqueName) : Gear(schema, "VideoRecord", uniqueName)
{
  // Inputs.
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_RECORD = new PlugIn<ValueType>(this, "Record", new ValueType(0, 0, 1)));
  addPlug(_RESET = new PlugIn<ValueType>(this, "Reset", new ValueType(0, 0, 1)));
  addPlug(_SEEK = new PlugIn<ValueType>(this, "Seek", new ValueType(0, -125, 125)));
  addPlug(_MEMORY = new PlugIn<ValueType>(this, "Memory", new ValueType(125, 0, 125)));

  EnumType *playbackMode = new EnumType(N_PLAYBACK_MODE, FORWARD);
  playbackMode->setLabel(FORWARD,"Foward");
  playbackMode->setLabel(BACKWARD,"Backward");
  playbackMode->setLabel(PING_PONG,"Ping pong");
  addPlug(_MODE = new PlugIn<EnumType>(this, "Mode", playbackMode));

  // Outputs.
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));

  // Internal objects.
  _circbuf = new CircularBuffer<RGBA>(BLACK_RGBA);
}

Gear_VideoRecord::~Gear_VideoRecord()
{
}

void Gear_VideoRecord::init()
{
  _currentLoopFrame = 0;
  _nLoopFrames = 0;
}

bool Gear_VideoRecord::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_VideoRecord::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  _playbackMode = CLAMP((ePlaybackMode)_MODE->type()->value(), FORWARD, PING_PONG);
  _memory = MAX(_MEMORY->type()->intValue(), 0);

  // Resize circular buffer to memory size.
  _circbuf->resize((int)_image->size(), _memory);

  // Reset switch.
  if ((int)_RESET->type()->value() == 1)
  {
    NOTICE("Reset");
    _currentLoopFrame = 0;
    _nLoopFrames = 0;
  }

  // Record switch.
  if ((int)_RECORD->type()->value() == 1)
  {
    // Now recording...
    _circbuf->append(_image->data()); // append current image
    _nLoopFrames++; // update number of frames
  }

  // Make sure number of frames fit in memory
  _nLoopFrames = MIN(_nLoopFrames,_memory);

  // Playback.
  if (_nLoopFrames > 0)
  {
    // Play it.
    switch (_playbackMode)
    {
    case FORWARD:
      _currentSeekFrame = REPEAT_CLAMP(_currentLoopFrame + _SEEK->type()->intValue(), 0, _nLoopFrames-1);
      _currentLoopFrame = (_currentLoopFrame + 1 % _nLoopFrames);
      break;
    case BACKWARD:
      _currentSeekFrame = REPEAT_CLAMP(_currentLoopFrame + _SEEK->type()->intValue(), 0, _nLoopFrames-1);
      _currentLoopFrame = (_currentLoopFrame==0? _nLoopFrames-1 : _currentLoopFrame-1);
      break;
    case PING_PONG:
      _currentSeekFrame = MIRROR_CLAMP(_currentLoopFrame + _SEEK->type()->intValue(), 0, _nLoopFrames-1);
      _currentLoopFrame = (_currentLoopFrame + 1 % _nLoopFrames);
      break;
    }

    // Fill output image with current frame.
    _circbuf->fillVectorFromBlock(_outImage, _currentSeekFrame -_nLoopFrames + 1);
  }
  else // default: play input image
    std::copy(_image->begin(), _image->end(), _outImage->begin());
  
}
