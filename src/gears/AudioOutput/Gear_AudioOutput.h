/* Gear_AudioOutput.h
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

#ifndef GEAR_AUDIOOUTPUT_INCLUDED
#define GEAR_AUDIOOUTPUT_INCLUDED

#include "Gear.h"
#include "SignalType.h"

extern "C" {
  #include <gst/gst.h>
  #include <gst/app/gstappsrc.h>
}

class PlugSignal;

class Gear_AudioOutput : public Gear  
{
public:

//CONFIG PARAMETERS
  static const QString SETTING_FRAMES_PER_BUFFER;
  static const QString SETTING_NB_BUFFERS;
//

  Gear_AudioOutput();
  virtual ~Gear_AudioOutput();

  void runAudio();

public:
//  struct GstFeedData {
//    GstElement *audioSource;
//    PlugIn<SignalType> *plug;
//    GstFeedData() : audioSource(NULL), plug(NULL) {}
//  };
//
//  static void gstStartFeedCallback(GstElement *source, guint size, GstFeedData *data);
//  static void gstStopFeedCallback(GstElement *source, guint size, GstFeedData *data);

  static void gstNeedsDataCallback(GstElement *source, guint size, Gear_AudioOutput *gear);
  static void gstEnoughDataCallback(GstElement *source, guint size, Gear_AudioOutput *gear);
  void needsData(bool needsIt) {
    _needsData = needsIt;
  }

protected:
  void internalPrePlay();    
  void internalPostPlay();    
  void internalInit();
  
  void onUpdateSettings();

  bool initOutput();
  void freeResources();
//  void init();

private:
  bool _setPlayState(bool play);

private:

//PLUGS
  PlugIn<SignalType> *_AUDIO_IN_LEFT;
  PlugIn<SignalType> *_AUDIO_IN_RIGHT;

  static const int DEFAULT_FRAMES_PER_BUFFER;
  static const int DEFAULT_NB_BUFFERS;

  // gstreamer
  GstBus *_bus;
  GstElement *_pipeline;
  GstElement *_audioSource;
  GstElement *_audioSink;

  int _testIter;

//  GstFeedData _feedData;

  bool _needsData;
};

#endif

