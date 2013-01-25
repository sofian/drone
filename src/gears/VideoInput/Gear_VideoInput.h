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
#include "VideoRGBAType.h"
#include "Utils.h"

extern "C" {
  #include <gst/gst.h>
  #include <gst/app/gstappsink.h>
}

class Gear_VideoInput : public Gear
{
public:
  Gear_VideoInput();
  virtual ~Gear_VideoInput();

  void runVideo();
  //void runAudio();

protected:
  bool initCamera();
  void freeResources();

  virtual void internalPrePlay();
  virtual void internalPostPlay();

private:
  bool _videoPull();
  bool _eos() const;

public:
  // GStreamer callback that simply sets the #newBuffer# flag to point to TRUE.
  static void gstNewBufferCallback(GstElement *sink, bool *newBuffer);

private:
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  VideoRGBAType *_imageOut;
	
  // gstreamer
  GstBus *_bus;
  GstElement *_pipeline;
  GstElement *_source;
  GstElement *_videoQueue;
  GstElement *_videoColorSpace;
  GstElement *_videoSink;

//  bool _audioHasNewBuffer;
  bool _videoHasNewBuffer;

  bool _terminate;
  bool _cameraReady;
};

#endif
