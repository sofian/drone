/* Gear_VideoSource.h
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

#ifndef GEAR_VIDEOSOURCE_INCLUDED
#define GEAR_VIDEOSOURCE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "SignalType.h"

#include <libmpeg3.h>

class Gear_VideoSource : public Gear
{
public:
  static const std::string SETTING_FILENAME;

  Gear_VideoSource(Engine *engine, std::string name);
  virtual ~Gear_VideoSource();

  void runVideo();
  void runAudio();

  PlugOut<VideoRGBAType>* VIDEO_OUT(){return _VIDEO_OUT;};
  PlugOut<SignalType>* AUDIO_OUT(){return _AUDIO_OUT;};

  bool ready();

protected:
  void onUpdateSettings();

private:

  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<SignalType> *_AUDIO_OUT;
  mpeg3_t* _file;

  int _sizeX, _sizeY;

  //locals
  
  float *_audioBuffer;
  RGBA *_frame[1024];
  RGBA *_outData;

  //mmx
//  unsigned long long int *_mmxImageIn;
//  unsigned long long int *_mmxImageOut;

};

#endif
