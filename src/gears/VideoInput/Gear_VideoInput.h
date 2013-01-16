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
#include <pthread.h>
#include <map>

#include "VideoRGBAType.h"

extern "C" {

#include <linux/videodev.h>
#define HAVE_STDINT_H
//#include <liblavrec.h>
#undef HAVE_STDINT_H
}

class Gear_VideoInput : public Gear
{
public:
  static const QString SETTING_DEVICE;
  static const QString DEFAULT_DEVICE;

  static const QString SETTING_WIDTH;
  static const QString SETTING_HEIGHT;
  static const int DEFAULT_WIDTH;
  static const int DEFAULT_HEIGHT;


  Gear_VideoInput();
  virtual ~Gear_VideoInput();

  void runVideo();  

  PlugOut<VideoRGBAType> *VIDEO_OUT(){return _VIDEO_OUT;};

  static void *playThread(void *parent);

protected:
  void internalInit();
  void internalPrePlay();
  void internalPostPlay();
  
  void onUpdateSettings();
//  static void videoCallback(uint8_t *video, long size, long count);

private:

  void initInputDevice();
  void resetInputDevice();

  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  int _sizeX, _sizeY;

  //v4l
  int _device;
  struct video_capability _vidCap;
  struct video_window _vidWin;
  struct video_picture _vidPic;
  struct video_clip _vidClips[32];
  struct video_capture _vidCapture;  
  struct video_channel _vidChannel;

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

  pthread_t _playThreadHandle;
  pthread_mutex_t *_mutex;
  bool _frameGrabbed;
  bool _playing;
                                    
  static std::list<QString> _lockedDevices;
  QString _ownedDevice;

};

#endif
