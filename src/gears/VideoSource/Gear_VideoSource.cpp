/* Gear_VideoSource.cpp
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
//inspired from Using libavformat and libavcodec by Martin B�hme (boehme@inb.uni-luebeckREMOVETHIS.de) 


#include <iostream>
#include "Gear_VideoSource.h"
#include "Engine.h"

#include "GearMaker.h"

#define SAMPLE_RATE 44100 /* Samples per second we are sending */
#define AUDIO_CAPS "audio/x-raw-float,channels=2,rate=%d,width=32,endianness=BYTE_ORDER"

#define WIDTH 720
#define HEIGHT 480
#define VIDEO_CAPS "video/x-raw-rgb,width=%d,height=%d"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_VideoSource(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "VideoSource";
    gearInfo.classification = GearClassifications::video().IO().instance();
    return gearInfo;
  }
}

const std::string Gear_VideoSource::SETTING_FILENAME = "Filename";

/* This function will be called by the pad-added signal */
void pad_added_handler (GstElement *src, GstPad *new_pad, Gear_VideoSource::GstPadHandlerData* data) {
  GstPadLinkReturn ret;
  GstCaps *new_pad_caps = NULL;
  GstStructure *new_pad_struct = NULL;
  const gchar *new_pad_type = NULL;

  bool isAudio = false;

  g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

  GstPad *sink_pad = 0;

  /* Check the new pad's type */
  new_pad_caps = gst_pad_get_caps (new_pad);
  new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
  new_pad_type = gst_structure_get_name (new_pad_struct);
  g_print("Structure is %s\n", gst_structure_to_string(new_pad_struct));
//  GST_LOG ("structure is %" GST_PTR_FORMAT, new_pad_struct);
  if (g_str_has_prefix (new_pad_type, "audio/x-raw")
      )
  {
    sink_pad = gst_element_get_static_pad (data->audioToConnect, "sink");
    isAudio = true;
  }
  else if (g_str_has_prefix (new_pad_type, "video/x-raw"))
  {
    sink_pad = gst_element_get_static_pad (data->videoToConnect, "sink");
    isAudio = false;
  }
  else {
    g_print ("  It has type '%s' which is not raw audio/video. Ignoring.\n", new_pad_type);
    goto exit;
  }

  /* If our converter is already linked, we have nothing to do here */
  if (gst_pad_is_linked (sink_pad)) {
    g_print ("  We are already linked. Ignoring.\n");
    goto exit;
  }

  /* Attempt the link */
  ret = gst_pad_link (new_pad, sink_pad);
  if (GST_PAD_LINK_FAILED (ret)) {
    g_print ("  Type is '%s' but link failed.\n", new_pad_type);
    goto exit;
  } else {
    g_print ("  Link succeeded (type '%s').\n", new_pad_type);
    if (isAudio)
      data->audioIsConnected = true;
    else
      data->videoIsConnected = true;
  }

exit:
  /* Unreference the new pad's caps, if we got them */
  if (new_pad_caps != NULL)
    gst_caps_unref (new_pad_caps);

  /* Unreference the sink pad */
  if (sink_pad != NULL)
    gst_object_unref (sink_pad);
}

void videoCopy(unsigned char* out, const unsigned char* in) {
  int size  = WIDTH*HEIGHT;
  for (int i=0;i<size;i++)
  {
    *out++ = *in++;
    *out++ = *in++;
    *out++ = *in++;
    *out++ = 255;
  }
}

/* The appsink has received a buffer */
void newVideoBufferCallback (GstElement *sink, VideoRGBAType* video) {
  GstBuffer *buffer;

  /* Retrieve the buffer */
  g_signal_emit_by_name (sink, "pull-buffer", &buffer);
  if (buffer) {
    std::cout << "v";
    videoCopy((unsigned char*)video->data(), GST_BUFFER_DATA(buffer));
    gst_buffer_unref (buffer);
  }
}

void newAudioBufferCallback (GstElement *sink, SignalType* audio) {
  std::cout << ".";
}

void newBufferCallback(GstElement *sink, bool *newBuffer) {
  std::cout << "New buffer" <<std::endl;
  *newBuffer = true;
}

Gear_VideoSource::Gear_VideoSource(Schema *schema, std::string uniqueName) : 
Gear(schema, "VideoSource", uniqueName),
_currentMovie(""),
_bus(NULL),
_pipeline(NULL),
_source(NULL),
_audioQueue(NULL),
_audioConvert(NULL),
_audioResample(NULL),
_videoQueue(NULL),
_videoConvert(NULL),
_videoColorSpace(NULL),
_audioSink(NULL),
_videoSink(NULL),
_audioHasNewBuffer(false),
_videoHasNewBuffer(false),
_movieReady(false)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut", false));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "AudioOut", false));

  addPlug(_FINISH_OUT = new PlugOut<ValueType>(this, "FinishOut", false));
  
  std::vector<AbstractPlug*> atLeastOneOfThem;
  atLeastOneOfThem.push_back(_VIDEO_OUT);
  atLeastOneOfThem.push_back(_AUDIO_OUT);
  setPlugAtLeastOneNeeded(atLeastOneOfThem);

  addPlug(_RESET_IN = new PlugIn<ValueType>(this, "Reset", false, new ValueType(0, 0, 1)));
  addPlug(_MOVIE_IN = new PlugIn<StringType>(this, "Movie", false));

  //_settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    

//  av_register_all();
	_VIDEO_OUT->sleeping(true);
	_AUDIO_OUT->sleeping(true);
}

Gear_VideoSource::~Gear_VideoSource()
{
  freeResources();
}

void Gear_VideoSource::freeResources()
{
//  if (_buffer)
//    delete [] _buffer;
  
  /* Free resources */
  if (_bus)
    gst_object_unref (_bus);

  if (_pipeline)
  {
    gst_element_set_state (_pipeline, GST_STATE_NULL);
    gst_object_unref (_pipeline);
  }

  _movieReady=false;
	_VIDEO_OUT->sleeping(true);
	unSynch();
}

bool Gear_VideoSource::loadMovie(std::string filename)
{
  std::cout << "opening movie : " << filename << std::endl;

  //free previously allocated structures
  freeResources();

  _audioHasNewBuffer = false;
  _videoHasNewBuffer = false;

  //_firstFrameTime=_formatContext->start_time;

  _movieReady=true;
	_VIDEO_OUT->sleeping(false);
	
  GstStateChangeReturn ret;
  gchar *video_caps_text;
  GstCaps *video_caps;

  /* Initialize GStreamer */
  gst_init (NULL, NULL);

  /* Create the elements */
  _source =          gst_element_factory_make ("uridecodebin", "source");

  _audioQueue =      gst_element_factory_make ("queue", "aqueue");
  _audioConvert =    gst_element_factory_make ("audioconvert", "aconvert");
  _audioResample =   gst_element_factory_make ("audioresample", "aresample");
  _audioSink =       gst_element_factory_make ("appsink", "asink");

  _videoQueue =      gst_element_factory_make ("queue", "vqueue");
  _videoConvert =    gst_element_factory_make ("autovideoconvert", "vconvert");
  _videoColorSpace = gst_element_factory_make ("ffmpegcolorspace", "vcolorspace");
  _videoSink =       gst_element_factory_make ("appsink", "vsink");

  _padHandlerData.audioToConnect = _audioQueue;
  _padHandlerData.videoToConnect = _videoQueue;
  _padHandlerData.audioIsConnected = _padHandlerData.videoIsConnected = false;

  /* Create the empty pipeline */
  _pipeline = gst_pipeline_new ("test-pipeline");

  if (!_pipeline || !_source ||
      !_audioQueue || !_audioConvert || !_audioResample || !_audioSink ||
      !_videoQueue || !_videoConvert ||  !_videoColorSpace || !_videoSink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Build the pipeline. Note that we are NOT linking the source at this
   * point. We will do it later. */
  gst_bin_add_many (GST_BIN (_pipeline), _source,
                    _audioQueue, _audioConvert, _audioResample, _audioSink,
                    _videoQueue, _videoConvert, _videoColorSpace, _videoSink, NULL);

  if (!gst_element_link (_audioQueue, _audioConvert) ||
      !gst_element_link (_audioConvert, _audioResample) ||
      !gst_element_link (_audioResample, _audioSink)) {
    g_printerr ("Audio elements could not be linked.\n");
    gst_object_unref (_pipeline);
    return false;
  }

  if (!gst_element_link (_videoQueue, _videoConvert) ||
      !gst_element_link (_videoConvert, _videoColorSpace) ||
//      !gst_element_link (_videoQueue, _videoColorSpace) ||
      !gst_element_link (_videoColorSpace, _videoSink)) {
    g_printerr ("Video elements could not be linked.\n");
    gst_object_unref (_pipeline);
    return false;
  }

  /* Set the URI to play */
  //g_object_set (_source, "uri", "file:///home/tats/Documents/workspace/drone-0.3/GOPR1063-h264.mp4", NULL);
  //g_object_set (_source, "uri", "file:///home/tats/Videos/GoPro-CubaJuin2012/GOPR1063-h264.mp4", NULL);

  // Process URI.
  gchar* uri = (gchar*) filename.c_str();
  if (!gst_uri_is_valid(uri))
  {
    // Try to convert filename to URI.
    GError* error = NULL;
    uri = gst_filename_to_uri(filename.c_str(), &error);
    if (error) {
      std::cout << "Filename to URI error: " << error->message << std::endl;
      g_error_free(error);
      gst_object_unref (uri);
      return false;
    }
  }

  g_object_set (_source, "uri", uri, NULL);

  //g_object_set (_source, "uri", "http://docs.gstreamer.com/media/sintel_trailer-480p.webm", NULL);

  /* Connect to the pad-added signal */
  g_signal_connect (_source, "pad-added", G_CALLBACK (pad_added_handler), &_padHandlerData);

  /* Configure audio appsink */
  gchar *audio_caps_text;
  GstCaps *audio_caps;

  audio_caps_text = g_strdup_printf (AUDIO_CAPS, SAMPLE_RATE);
  audio_caps = gst_caps_from_string (audio_caps_text);
  g_object_set (_audioSink, "emit-signals", TRUE, "caps", audio_caps, NULL);
  //g_signal_connect (_audioSink, "new-buffer", G_CALLBACK (newBufferCallback), &_audioHasNewBuffer);
  g_signal_connect (_audioSink, "new-buffer", G_CALLBACK (newAudioBufferCallback), _AUDIO_OUT->type());
  gst_caps_unref (audio_caps);
  g_free (audio_caps_text);

  /* Configure video appsink */
  video_caps_text = g_strdup_printf (VIDEO_CAPS, WIDTH, HEIGHT);
  video_caps = gst_caps_from_string (video_caps_text);
  g_object_set (_videoSink, "emit-signals", TRUE, "caps", video_caps, NULL);
  g_signal_connect (_videoSink, "new-buffer", G_CALLBACK (newBufferCallback), &_videoHasNewBuffer);
  //g_signal_connect (_videoSink, "new-buffer", G_CALLBACK (newVideoBufferCallback), _VIDEO_OUT->type());
  gst_caps_unref (video_caps);
  g_free (video_caps_text);

  /* Start playing */
  ret = gst_element_set_state (_pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    gst_object_unref (_pipeline);
    return -1;
  }

  /* Listen to the bus */
  _bus = gst_element_get_bus (_pipeline);

  _terminate = false;

	return true;
}


void Gear_VideoSource::runVideo() {
//  int frameFinished=0;

  if (_currentMovie != _MOVIE_IN->type()->value()) {
    _currentMovie = _MOVIE_IN->type()->value();
    if (!loadMovie(_currentMovie))
      return;
  }

  if (!_movieReady)
    return;

  if (!_padHandlerData.isConnected())
    return;

  if (_terminate) {
    _FINISH_OUT->type()->setValue(1.0f);
    return;
  }

  // TODO: resize according to caps
  _VIDEO_OUT->type()->resize(WIDTH, HEIGHT);

  // TODO: make this work
  if ((int) _RESET_IN->type()->value() == 1) {
    std::cout << "Seeking not implemented" << std::endl;
//    av_seek_frame(_formatContext, -1, _formatContext->start_time, AVSEEK_FLAG_BACKWARD);
  }

  //loop until we get a videoframe
  //if we reach end, return to the beginning

  // TODO: make this work (EOS)
  if (false/*gst_app_sink_is_eos(_videoSink)*/) {
    _FINISH_OUT->type()->setValue(1.0f);
  } else {
    _FINISH_OUT->type()->setValue(0.0f);

    //GstMessage *msg = gst_bus_timed_pop_filtered(
//                        _bus, GST_CLOCK_TIME_NONE,
//                        (GstMessageType) (GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    GstMessage *msg = gst_bus_timed_pop_filtered(
                        _bus, 0,
                        (GstMessageType) (GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    if (_videoHasNewBuffer) {
      std::cout << "Supposedly has video buff" << std::endl;
      newVideoBufferCallback(_videoSink, _VIDEO_OUT->type());
      //      GstBuffer* buffer;
      //      g_signal_emit_by_name (_videoSink, "pull-buffer", &buffer);
      //
      //      if (buffer) {
      //        std::cout << "Has buffer\n" << std::endl;
      //        _videoHasNewBuffer = false;
    }
//    if (_videoHasNewBuffer) {
//      GstBuffer* buffer;
//      g_signal_emit_by_name (_videoSink, "pull-buffer", &buffer);
//
//      if (buffer) {
//        std::cout << "Has buffer\n" << std::endl;
//        _videoHasNewBuffer = false;
///*
//        register unsigned char *out = (unsigned char*) _VIDEO_OUT->type()->data();
//        register unsigned char *in  = (unsigned char*) GST_BUFFER_DATA(buffer);
//        register int size  = WIDTH*HEIGHT;
//        for (register int i=0;i<size;i++)
//        {
//          *out++ = *in++;
//          *out++ = *in++;
//          *out++ = *in++;
//          *out++ = 255;
//        }
//        */
//      }

    /* Parse message */
    if (msg != NULL) {
      GError *err;
      gchar *debug_info;

      switch (GST_MESSAGE_TYPE (msg)) {
      case GST_MESSAGE_ERROR:
        gst_message_parse_error(msg, &err, &debug_info);
        g_printerr("Error received from element %s: %s\n",
            GST_OBJECT_NAME (msg->src), err->message);
        g_printerr("Debugging information: %s\n",
            debug_info ? debug_info : "none");
        g_clear_error(&err);
        g_free(debug_info);
        _terminate = true;
        _FINISH_OUT->type()->setValue(1.0f);
        //          terminate = TRUE;
        break;
      case GST_MESSAGE_EOS:
        g_print("End-Of-Stream reached.\n");
        _terminate = TRUE;
        _FINISH_OUT->type()->setValue(1.0f);
        break;
      case GST_MESSAGE_STATE_CHANGED:
        /* We are only interested in state-changed messages from the pipeline */
        if (GST_MESSAGE_SRC (msg) == GST_OBJECT (_pipeline)) {
          GstState old_state, new_state, pending_state;
          gst_message_parse_state_changed(msg, &old_state, &new_state,
              &pending_state);
          g_print("Pipeline state changed from %s to %s:\n",
              gst_element_state_get_name(old_state),
              gst_element_state_get_name(new_state));
        }
        break;
      default:
        /* We should not reach here */
        g_printerr("Unexpected message received.\n");
        break;
      }
      gst_message_unref(msg);
    }
  }
}



