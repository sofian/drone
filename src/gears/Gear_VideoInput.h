/* Gear_VideoInput.h
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

#ifndef GEAR_VIDEOINPUT_INCLUDED
#define GEAR_VIDEOINPUT_INCLUDED


#include "Gear.h"

#include "VideoType.h"

extern "C" {

#include <linux/videodev.h>
#define HAVE_STDINT_H
//#include <liblavrec.h>
#undef HAVE_STDINT_H
}

class Gear_VideoInput : public Gear
{
public:
  static const std::string SETTING_DEVICE;
  static const std::string DEFAULT_DEVICE;

  Gear_VideoInput(Engine *engine, std::string name);
  virtual ~Gear_VideoInput();

  void runVideo();
  void init();
  

  PlugOut<VideoTypeRGBA> *VIDEO_OUT(){return _VIDEO_OUT;};

  bool ready();

protected:
  void onUpdateSettings();
//  static void videoCallback(uint8_t *video, long size, long count);

private:

  void initInputDevice();
  void resetInputDevice();

  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;

  int _sizeX, _sizeY;

  //v4l
  int _device;
  struct video_capability _vidCap;
  struct video_window _vidWin;
  struct video_picture _vidPic;
  struct video_clip _vidClips[32];

  //mmap
  struct video_mbuf _vidMBuf;    
  struct video_mmap _vidMMap;    
  struct video_buffer _vidBuf;
  unsigned char *_bufferBGRA;


  //locals
  unsigned char *_outData;
  unsigned char *_tempOutData;
  unsigned char *_tempInData;


  //mmx
  double *_mmxImageIn;
  double *_mmxImageOut;

  //lavrec_t *_lavrecInfo;
  static unsigned char *_data;

};

#endif
