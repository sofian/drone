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

#define WIDTH 320
#define HEIGHT 240
#define VIDEO_CAPS "video/x-raw-rgb"
//#define VIDEO_CAPS "video/x-raw-rgb,width=%d,height=%d"

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

//const std::string Gear_VideoSource::SETTING_FILENAME = "Filename";

void Gear_VideoSource::gstPadAddedCallback(GstElement *src, GstPad *newPad, Gear_VideoSource::GstPadHandlerData* data) {
  g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (newPad), GST_ELEMENT_NAME (src));
  bool isAudio = false;
  GstPad *sinkPad = NULL;

  /* Check the new pad's type */
  GstCaps *newPadCaps   = gst_pad_get_caps (newPad);
  GstStructure *newPadStruct = gst_caps_get_structure (newPadCaps, 0);
  const gchar *newPadType   = gst_structure_get_name (newPadStruct);
  g_print("Structure is %s\n", gst_structure_to_string(newPadStruct));
  if (g_str_has_prefix (newPadType, "audio/x-raw"))
  {
    sinkPad = gst_element_get_static_pad (data->audioToConnect, "sink");
    isAudio = true;
  }
  else if (g_str_has_prefix (newPadType, "video/x-raw"))
  {
    sinkPad = gst_element_get_static_pad (data->videoToConnect, "sink");
    isAudio = false;
  }
  else
  {
    g_print ("  It has type '%s' which is not raw audio/video. Ignoring.\n", newPadType);
    goto exit;
  }

  /* If our converter is already linked, we have nothing to do here */
  if (gst_pad_is_linked (sinkPad)) {
    g_print ("  We are already linked. Ignoring.\n");
    goto exit;
  }

  /* Attempt the link */
  if (GST_PAD_LINK_FAILED (gst_pad_link (newPad, sinkPad))) {
    g_print ("  Type is '%s' but link failed.\n", newPadType);
    goto exit;
  } else {
    g_print ("  Link succeeded (type '%s').\n", newPadType);
    if (isAudio)
    {
      data->audioIsConnected = true;
    }
    else
    {
      data->videoIsConnected = true;
    }
  }

exit:
  /* Unreference the new pad's caps, if we got them */
  if (newPadCaps != NULL)
    gst_caps_unref (newPadCaps);

  /* Unreference the sink pad */
  if (sinkPad != NULL)
    gst_object_unref (sinkPad);
}

/* The appsink has received a buffer */
void Gear_VideoSource::_gstVideoPull()
{
  GstBuffer *buffer;

  /* Retrieve the buffer */
  g_signal_emit_by_name (_videoSink, "pull-buffer", &buffer);

  if (buffer) {

    GstCaps* caps = GST_BUFFER_CAPS(buffer);
    GstStructure *caps_struct = gst_caps_get_structure (caps, 0);
    VideoRGBAType* video = _VIDEO_OUT->type();

    int width  = video->width();
    int height = video->height();
    int bpp    = 24;
    int depth  = 24;

    gst_structure_get_int(caps_struct, "width",  &width);
    gst_structure_get_int(caps_struct, "height", &height);
    gst_structure_get_int(caps_struct, "bpp",    &bpp);
    gst_structure_get_int(caps_struct, "depth",  &depth);

    video->resize(width, height);

    convert24to32((unsigned char*)video->data(), GST_BUFFER_DATA(buffer), video->size());
// Make sure the buffer width / height are right.
//    int nPixelsInBuffer = GST_BUFFER_SIZE(buffer) / (bpp / 8);
//    int nPixels = width * height;
//    if (nPixelsInBuffer != nPixels) {
//      std::cout << "Improper number of pixels : " << nPixelsInBuffer << " != " << nPixels << std::endl;
//      std::cout << GST_BUFFER_OFFSET(buffer) << " " << GST_BUFFER_OFFSET_END(buffer) <<
//          " " << (GST_BUFFER_OFFSET_END(buffer) - GST_BUFFER_OFFSET(buffer)) << " " << std::endl;
//      width += (nPixelsInBuffer - nPixels) / height;
//    }
//
//    std::cout << gst_structure_to_string(caps_struct) << std::endl;
//    std::cout << width << "x" << height << "=" << width*height << "(" << width*height*4 << "," << width*height*3 << ")" << std::endl;
//    std::cout << "bpp: " << bpp << " depth: " << depth << std::endl;
//    std::cout << "Buffer size: " << GST_BUFFER_SIZE(buffer) << std::endl;

    //rgb2rgba(video->data(), (const RGB*)GST_BUFFER_DATA(buffer), width*height);

    gst_buffer_unref (buffer);
  }
}

bool Gear_VideoSource::_eos() const
{
  if (_movieReady) {
    gboolean eos;
    g_object_get (G_OBJECT (_videoSink), "eos", &eos, NULL);
    return (bool)eos;
  }
  else
    return false;
}


void Gear_VideoSource::gstNewBufferCallback(GstElement*, bool *newBuffer)
{
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
_seekEnabled(false),
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

void Gear_VideoSource::resetMovie()
{
  if (!_eos() && _seekEnabled)
  {
    gst_element_seek_simple (_pipeline, GST_FORMAT_TIME,
                             (GstSeekFlags) (GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), 0);
  }
  else
  {
    // Just reload movie.
    std::cout << "Just reload movie" << std::endl;
    _currentMovie = "";
  }
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

  /* Initialize GStreamer */
  gst_init (NULL, NULL);

  /* Create the elements */
  _source =          gst_element_factory_make ("uridecodebin", "source");

  _audioQueue =      gst_element_factory_make ("queue", "aqueue");
  _audioConvert =    gst_element_factory_make ("audioconvert", "aconvert");
  _audioResample =   gst_element_factory_make ("audioresample", "aresample");
  _audioSink =       gst_element_factory_make ("appsink", "asink");

  _videoQueue =      gst_element_factory_make ("queue", "vqueue");
  //_videoConvert =    gst_element_factory_make ("autovideoconvert", "vconvert");
  _videoColorSpace = gst_element_factory_make ("ffmpegcolorspace", "vcolorspace");
  _videoSink =       gst_element_factory_make ("appsink", "vsink");

  // Prepare handler data.
  _padHandlerData.audioToConnect   = _audioQueue;
  _padHandlerData.videoToConnect   = _videoQueue;
  _padHandlerData.videoSink        = _videoSink;
  _padHandlerData.audioIsConnected = _padHandlerData.videoIsConnected = false;

  // Create the empty pipeline.
  _pipeline = gst_pipeline_new ( "video-source-pipeline" );

  if (!_pipeline || !_source ||
      !_audioQueue || !_audioConvert || !_audioResample || !_audioSink ||
      !_videoQueue || !_videoColorSpace || !_videoSink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  // Build the pipeline. Note that we are NOT linking the source at this
  // point. We will do it later.
  gst_bin_add_many (GST_BIN (_pipeline), _source,
                    _audioQueue, _audioConvert, _audioResample, _audioSink,
                    _videoQueue, _videoColorSpace, _videoSink, NULL);

  if (!gst_element_link (_audioQueue, _audioConvert) ||
      !gst_element_link (_audioConvert, _audioResample) ||
      !gst_element_link (_audioResample, _audioSink)) {
    g_printerr ("Audio elements could not be linked.\n");
    gst_object_unref (_pipeline);
    return false;
  }

  if (!gst_element_link (_videoQueue, _videoColorSpace) ||
      !gst_element_link (_videoColorSpace, _videoSink)) {
    g_printerr ("Video elements could not be linked.\n");
    gst_object_unref (_pipeline);
    return false;
  }

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

  // Set URI to be played.
  g_object_set (_source, "uri", uri, NULL);

  // Connect to the pad-added signal
  g_signal_connect (_source, "pad-added", G_CALLBACK (Gear_VideoSource::gstPadAddedCallback), &_padHandlerData);

  // Configure audio appsink.
  gchar* audio_caps_text = g_strdup_printf (AUDIO_CAPS, SAMPLE_RATE);
  GstCaps* audio_caps = gst_caps_from_string (audio_caps_text);
  g_object_set (_audioSink, "emit-signals", TRUE, "caps", audio_caps, NULL);
  g_signal_connect (_audioSink, "new-buffer", G_CALLBACK (Gear_VideoSource::gstNewBufferCallback), &_audioHasNewBuffer);
  //g_signal_connect (_audioSink, "new-buffer", G_CALLBACK (newAudioBufferCallback), _AUDIO_OUT->type());
  gst_caps_unref (audio_caps);
  g_free (audio_caps_text);

  // Configure video appsink.
  gchar *video_caps_text = g_strdup_printf (VIDEO_CAPS);
  GstCaps *video_caps = gst_caps_from_string (video_caps_text);
  //g_object_set (_videoSink, "emit-signals", TRUE, NULL);
  g_object_set (_videoSink, "emit-signals", TRUE, "caps", video_caps, NULL);
  g_signal_connect (_videoSink, "new-buffer", G_CALLBACK (Gear_VideoSource::gstNewBufferCallback), &_videoHasNewBuffer);
  //g_signal_connect (_videoSink, "new-buffer", G_CALLBACK (newVideoBufferCallback), _VIDEO_OUT->type());
  gst_caps_unref (video_caps);
  g_free (video_caps_text);

  // Start playing.
  ret = gst_element_set_state (_pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    std::cout << "Unable to set the pipeline to the playing state." << std::endl;
    gst_object_unref (_pipeline);
    return false;
  }

  // Listen to the bus.
  _bus = gst_element_get_bus (_pipeline);

  _terminate = false;
  _seekEnabled = false;

	return true;
}


void Gear_VideoSource::runVideo() {

  if (_eos()) {
    _FINISH_OUT->type()->setValue(1.0f);
  }
  else
    _FINISH_OUT->type()->setValue(0.0f);

  if ((int) _RESET_IN->type()->value() == 1) {
    resetMovie();
  }

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

  {
    _FINISH_OUT->type()->setValue(0.0f);

    GstMessage *msg = gst_bus_timed_pop_filtered(
                        _bus, 0,
                        (GstMessageType) (GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    if (_videoHasNewBuffer) {
      _gstVideoPull();
      _videoHasNewBuffer = false;
    }

    // Parse message.
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

          if (new_state == GST_STATE_PLAYING) {
            // Check if seeking is allowed.
            gint64 start, end;
            GstQuery *query = gst_query_new_seeking (GST_FORMAT_TIME);
            if (gst_element_query (_pipeline, query))
            {
              gst_query_parse_seeking (query, NULL, (gboolean*)&_seekEnabled, &start, &end);
              if (_seekEnabled)
              {
                g_print ("Seeking is ENABLED from %" GST_TIME_FORMAT " to %" GST_TIME_FORMAT "\n",
                         GST_TIME_ARGS (start), GST_TIME_ARGS (end));
              }
              else
              {
                g_print ("Seeking is DISABLED for this stream.\n");
              }
            }
            else
            {
              g_printerr ("Seeking query failed.");
            }

            gst_query_unref (query);
          }
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



