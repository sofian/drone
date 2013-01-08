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
#include "StringType.h"
#include "EnumType.h"
#include "Utils.h"

extern "C" {
  #include <gst/gst.h>
  #include <gst/app/gstappsink.h>
}

class Gear_VideoSource : public Gear
{
public:
  /*
  enum ePlaybackMode
  {
    NORMAL,
    LOOP,
    N_PLAYBACK_MODE
  };*/
  
  //static const std::string SETTING_FILENAME;

  Gear_VideoSource(Schema *schema, std::string uniqueName);
  virtual ~Gear_VideoSource();

  void runVideo();
  void runAudio();

protected:
  bool loadMovie(std::string filename);
  void freeResources();
  void resetMovie();

  virtual void internalPrePlay();
  virtual void internalPostPlay();

private:
  bool _videoPull();
  bool _audioPull();
  bool _eos() const;
  void _postLoadOrResetMovie();

  void _preRun();
  void _postRun();
  void _setPlayState(bool play);

  //void gstAudioPull();

public:
  // GStreamer callbacks.

  struct GstPadHandlerData {
    GstElement* audioToConnect;
    GstElement* videoToConnect;
    GstElement* videoSink;
    bool audioIsConnected;
    bool videoIsConnected;

    GstPadHandlerData() :
      audioToConnect(NULL), videoToConnect(NULL), videoSink(NULL),
      audioIsConnected(false), videoIsConnected(false)
    {}

    bool isConnected() const {
      return (audioIsConnected && videoIsConnected);
    }
  };

  // GStreamer callback that simply sets the #newBuffer# flag to point to TRUE.
  static void gstNewBufferCallback(GstElement *sink, bool *newBuffer);

  // GStreamer callback that plugs the audio/video pads into the proper elements when they
  // are made available by the source.
  static void gstPadAddedCallback(GstElement *src, GstPad *newPad, Gear_VideoSource::GstPadHandlerData* data);

private:
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<SignalType> *_AUDIO_OUT;
	PlugOut<ValueType> *_FINISH_OUT;
  PlugIn<ValueType> *_RESET_IN;
  PlugIn<StringType> *_MOVIE_IN;

  VideoRGBAType *_imageOut;

  //locals
  std::string _currentMovie;  

  //float *_audioBuffer;
  //RGBA *_outData;  
  //long _previousFramePos;
	
  // gstreamer
  GstBus *_bus;
  GstElement *_pipeline;
  GstElement *_source;
  GstElement *_audioQueue;
  GstElement *_audioConvert;
  GstElement *_audioResample;
  GstElement *_videoQueue;
  GstElement *_videoConvert;
  GstElement *_videoColorSpace;
  GstElement *_audioSink;
  GstElement *_videoSink;

  GstPadHandlerData _padHandlerData;

  bool _seekEnabled;

  bool _audioHasNewBuffer;
  bool _videoHasNewBuffer;

  bool _terminate;
  bool _movieReady;
};

#endif
