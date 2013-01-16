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
Gear* makeGear()
{
  return new Gear_VideoRecord();
}


}

Gear_VideoRecord::Gear_VideoRecord() : Gear("VideoRecord")
{
  // Inputs.
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_RECORD_IN = new PlugIn<ValueType>(this, "Record", false, new ValueType(0, 0, 1)));
  addPlug(_MEMORY_IN = new PlugIn<ValueType>(this, "Memory", false, new ValueType(125, 0, 125)));
  addPlug(_FRAME_IN = new PlugIn<ValueType>(this, "Frame", false, new ValueType(0, 0, 0)));

  // Outputs.
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  addPlug(_N_FRAMES_OUT = new PlugOut<ValueType>(this, "NFrames", false));

  // Internal objects.
  _circbuf = new CircularBuffer<RGBA>(BLACK_RGBA);
}

Gear_VideoRecord::~Gear_VideoRecord()
{
}

void Gear_VideoRecord::internalInit()
{
  _nFrames = 0;
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

  int frame = _FRAME_IN->type()->intValue();
std::cerr << "!!!!!!!!!" << frame << std::endl;
  // Playback.
  if (_nFrames > 0 && frame>=0)
    // Fill output image with current frame.
    _circbuf->fillVectorFromBlock(_outImage, CLAMP(frame, 0, _nFrames-1) - _nFrames + 1);
  else // default: play input image
    //std::copy(_image->begin(), _image->end(), _outImage->begin());
    _outImage->fill(CLEAR_RGBA);

  // Output number of frames.
  _N_FRAMES_OUT->type()->setValue(_nFrames);
}
