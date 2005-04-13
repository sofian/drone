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
  addPlug(_RECORD_IN = new PlugIn<ValueType>(this, "Record", new ValueType(0, 0, 1)));
  addPlug(_MEMORY_IN = new PlugIn<ValueType>(this, "Memory", new ValueType(125, 0, 125)));
  addPlug(_FRAME_IN = new PlugIn<ValueType>(this, "Frame", new ValueType(0, 0, 0)));

  // Outputs.
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  addPlug(_N_FRAMES_OUT = new PlugOut<ValueType>(this, "NFrames"));

  // Internal objects.
  _circbuf = new CircularBuffer<RGBA>(BLACK_RGBA);
}

Gear_VideoRecord::~Gear_VideoRecord()
{
}

void Gear_VideoRecord::init()
{
  _nFrames = 0;
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

  _memory = MAX(_MEMORY_IN->type()->intValue(), 0);

  // Resize circular buffer to memory size.
  _circbuf->resize((int)_image->size(), _memory);

  // Record switch.
  if (_RECORD_IN->type()->intValue())
  {
    // Now recording...
    _circbuf->append(_image->data()); // append current image
    _nFrames++; // update number of frames
  }

  // Make sure number of frames fit in memory
  _nFrames = MIN(_nFrames,_memory);

  // Playback.
  if (_nFrames > 0)
    // Fill output image with current frame.
    _circbuf->fillVectorFromBlock(_outImage, CLAMP(_FRAME_IN->type()->intValue(), 0, _nFrames-1) - _nFrames + 1);
  else // default: play input image
    std::copy(_image->begin(), _image->end(), _outImage->begin());

  // Output number of frames.
  _N_FRAMES_OUT->type()->setValue(_nFrames);
}
