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
#include "ValueType.h"
#include "SignalType.h"
#include "EnumType.h"

#include "avcodec.h"
#include "avformat.h"

class Gear_VideoSource : public Gear
{
public:
  enum ePlaybackMode
  {
    NORMAL,
    LOOP,
    N_PLAYBACK_MODE
  };
  
  static const std::string SETTING_FILENAME;

  Gear_VideoSource(Schema *schema, std::string uniqueName);
  virtual ~Gear_VideoSource();

  void runVideo();

  bool ready();

protected:
  void onUpdateSettings();

private:

  void freeResources();

  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<SignalType> *_AUDIO_OUT;
  PlugIn<ValueType> *_RESET_IN;

  VideoRGBAType *_imageOut;

  //locals
  
  float *_audioBuffer;
  //RGBA *_outData;  
  long _previousFramePos;

  //ffmpeg
  AVFormatContext *_formatContext;  
  AVCodecContext *_codecContext;
  AVCodec *_codec;
  AVPacket _packet;
  AVFrame *_frame;
  AVFrame *_frameRGBA;
  uint8_t *_buffer;
  int _videoStreamIndex;
  int64_t _firstFrameTime;
  bool _movieReady;
};

#endif
