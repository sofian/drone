/* Gear_VideoSource.cpp
 * Copyright (C) 2012 Jean-Sebastien Senecal
 *           (C) 2004 Mathieu Guindon, Julien Keable
 *
 *           Based on code from the GStreamer Tutorials http://docs.gstreamer.com/display/GstSDK/Tutorials
 *
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


#include <iostream>
#include "Gear_VideoSource.h"
#include "Engine.h"



extern "C" {           
  Gear* makeGear()
  {
    return new Gear_VideoSource();
  }  
  
}

bool Gear_VideoSource::_videoPull()
{
  GstBuffer *buffer;

  // Retrieve the buffer.
  g_signal_emit_by_name (_videoSink, "pull-buffer", &buffer);

  if (!buffer)
  {
    // Either means we are not playing or we have reached EOS.
    return false;
  }

  else
  {
    GstCaps* caps = GST_BUFFER_CAPS(buffer);
    GstStructure *capsStruct = gst_caps_get_structure (caps, 0);
    VideoRGBAType* video = _VIDEO_OUT->type();

    int width  = video->width();
    int height = video->height();
    int bpp    = 24;
    int depth  = 24;

    gst_structure_get_int(capsStruct, "width",  &width);
    gst_structure_get_int(capsStruct, "height", &height);
    gst_structure_get_int(capsStruct, "bpp",    &bpp);
    gst_structure_get_int(capsStruct, "depth",  &depth);

    video->resize(width, height);

//        std::cout << gst_structure_to_string(capsStruct) << std::endl;
//        std::cout << width << "x" << height << "=" << width*height << "(" << width*height*4 << "," << width*height*3 << ")" << std::endl;
//        std::cout << "bpp: " << bpp << " depth: " << depth << std::endl;
//        std::cout << "Buffer size: " << GST_BUFFER_SIZE(buffer) << std::endl;

    if (bpp == 32)
      memcpy(video->data(), GST_BUFFER_DATA(buffer), video->size() * sizeof(RGBA));
    else
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
    return true;
  }

}

bool Gear_VideoSource::_eos() const
{
  if (_movieReady)
  {
    ASSERT_ERROR( _videoSink );
    ASSERT_ERROR( _audioSink );
    gboolean videoEos;
    gboolean audioEos;
    g_object_get (G_OBJECT (_videoSink), "eos", &videoEos, NULL);
    g_object_get (G_OBJECT (_audioSink), "eos", &audioEos, NULL);
    return (bool) (videoEos || audioEos);
  }
  else
    return false;
}

//void Gear_VideoSource::_init()
//{
//  _audioHasNewBuffer = false;
//  _videoHasNewBuffer = false;
//
//  _terminate = false;
//  _seekEnabled = false;
//
//  _movieReady=true;
//
//  // Stop sleeping the video output.
//  _VIDEO_OUT->sleeping(false);
//  _AUDIO_OUT->sleeping(false);
//}


void Gear_VideoSource::gstNewBufferCallback(GstElement*, int *newBufferCounter)
{
  (*newBufferCounter)++;
}

Gear_VideoSource::Gear_VideoSource() :
Gear("VideoSource"),
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
_audioBufferAdapter(NULL),
_seekEnabled(false),
_audioNewBufferCounter(0),
_videoNewBufferCounter(0),
_movieReady(false)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut", false));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "AudioOut", false));

  addPlug(_FINISH_OUT = new PlugOut<ValueType>(this, "FinishOut", false));
  
  QList<AbstractPlug*> atLeastOneOfThem;
  atLeastOneOfThem.push_back(_VIDEO_OUT);
  atLeastOneOfThem.push_back(_AUDIO_OUT);
  setPlugAtLeastOneNeeded(atLeastOneOfThem);

  addPlug(_RESET_IN = new PlugIn<ValueType>(this, "Reset", false, new ValueType(0, 0, 1)));
  addPlug(_MOVIE_IN = new PlugIn<StringType>(this, "Movie", false));

  //_settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    

	_VIDEO_OUT->sleeping(true);
	_AUDIO_OUT->sleeping(true);

  // Crease audio buffer handler.
  _audioBufferAdapter = gst_adapter_new();
}

Gear_VideoSource::~Gear_VideoSource()
{
  freeResources();
  gst_object_unref(_audioBufferAdapter);
}

void Gear_VideoSource::unloadMovie()
{
  // Free allocated resources.
  freeResources();

  // Reset flags.
  _audioNewBufferCounter = 0;
  _videoNewBufferCounter = 0;

  _terminate = false;
  _seekEnabled = false;

  _setReady(false);

  // Unsynch.
  unSynch(); // XXX: I'm not sure why we are doing this...
}

void Gear_VideoSource::freeResources()
{
  // Free resources.
  if (_bus)
  {
    gst_object_unref (_bus);
    _bus = NULL;
  }

  if (_pipeline)
  {
    gst_element_set_state (_pipeline, GST_STATE_NULL);
    gst_object_unref (_pipeline);
    _pipeline = NULL;
  }

  _source = NULL;
  _audioQueue = NULL;
  _audioConvert = NULL;
  _audioResample = NULL;
  _videoQueue = NULL;
  _videoConvert = NULL;
  _videoColorSpace = NULL;
  _audioSink = NULL;
  _videoSink = NULL;
  _padHandlerData = GstPadHandlerData();

  // Flush buffers in adapter.
  gst_adapter_clear(_audioBufferAdapter);

}

void Gear_VideoSource::resetMovie()
{
  // TODO: Check if we can still seek when we reach EOS. It seems like it's then impossible and we
  // have to reload but it seems weird so we should check.
  if (!_eos() && _seekEnabled)
  {
    std::cout << "Seeking at position 0" << std::endl;
    gst_element_seek_simple (_pipeline, GST_FORMAT_TIME,
                             (GstSeekFlags) (GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), 0);
    _setReady(true);
  }
  else
  {
    // Just reload movie.
    std::cout << "Reloading the movie" << std::endl;
    _currentMovie = "";
  }
}

bool Gear_VideoSource::loadMovie(QString filename)
{
  qDebug() << "Opening movie: " << filename << ".";

  // Free previously allocated structures
  unloadMovie();

  //_firstFrameTime=_formatContext->start_time;

  // Initialize GStreamer.
  gst_init (NULL, NULL);

  // Create the elements.
  _source =          gst_element_factory_make ("uridecodebin", "source");

  _audioQueue =      gst_element_factory_make ("queue", "aqueue");
  _audioConvert =    gst_element_factory_make ("audioconvert", "aconvert");
  _audioResample =   gst_element_factory_make ("audioresample", "aresample");
  _audioSink =       gst_element_factory_make ("appsink", "asink");

  _videoQueue =      gst_element_factory_make ("queue", "vqueue");
  _videoColorSpace = gst_element_factory_make ("ffmpegcolorspace", "vcolorspace");
  _videoSink =       gst_element_factory_make ("appsink", "vsink");

  // Prepare handler data.
  _padHandlerData.audioToConnect   = _audioQueue;
  _padHandlerData.videoToConnect   = _videoQueue;
  _padHandlerData.videoSink        = _videoSink;
  _padHandlerData.audioIsConnected = _padHandlerData.videoIsConnected = false;

  _newAudioBufferHandlerData.audioSink          = _audioSink;
  _newAudioBufferHandlerData.audioBufferAdapter = _audioBufferAdapter;

  // Create the empty pipeline.
  _pipeline = gst_pipeline_new ( "video-source-pipeline" );

  if (!_pipeline || !_source ||
      !_audioQueue || !_audioConvert || !_audioResample || !_audioSink ||
      !_videoQueue || !_videoColorSpace || !_videoSink) {
    g_printerr ("Not all elements could be created.\n");
    unloadMovie();
    return -1;
  }

  // Build the pipeline. Note that we are NOT linking the source at this
  // point. We will do it later.
  gst_bin_add_many (GST_BIN (_pipeline), _source,
                    _audioQueue, _audioConvert, _audioResample, _audioSink,
                    _videoQueue, _videoColorSpace, _videoSink, NULL);

  if (!gst_element_link_many(_audioQueue, _audioConvert, _audioResample, _audioSink, NULL)) {
    g_printerr ("Audio elements could not be linked.\n");
    unloadMovie();
    return false;
  }

  if (!gst_element_link_many (_videoQueue, _videoColorSpace, _videoSink, NULL)) {
    g_printerr ("Video elements could not be linked.\n");
    unloadMovie();
    return false;
  }

  // Process URI.
  gchar* uri = (gchar*) filename.ascii();
  if (!gst_uri_is_valid(uri))
  {
    // Try to convert filename to URI.
    GError* error = NULL;
    uri = gst_filename_to_uri(filename.ascii(), &error);
    if (error) {
      std::cout << "Filename to URI error: " << error->message << std::endl;
      g_error_free(error);
      gst_object_unref (uri);
      freeResources();
      return false;
    }
  }

  // Set URI to be played.
  g_object_set (_source, "uri", uri, NULL);

  // Connect to the pad-added signal
  g_signal_connect (_source, "pad-added", G_CALLBACK (Gear_VideoSource::gstPadAddedCallback), &_padHandlerData);

  // Configure audio appsink.
  // TODO: change from mono to stereo
  gchar* audioCapsText = g_strdup_printf ("audio/x-raw-float,channels=1,rate=%d,signed=(boolean)true,width=%d,depth=%d,endianness=BYTE_ORDER",
                                          Engine::signalInfo().sampleRate(), (int)(sizeof(Signal_T)*8), (int)(sizeof(Signal_T)*8) );
  GstCaps* audioCaps = gst_caps_from_string (audioCapsText);
  g_object_set (_audioSink, "emit-signals", TRUE,
                            "caps", audioCaps,
//                            "max-buffers", 1,     // only one buffer (the last) is maintained in the queue
//                            "drop", TRUE,         // ... other buffers are dropped
                            NULL);
  g_signal_connect (_audioSink, "new-buffer", G_CALLBACK (Gear_VideoSource::gstNewAudioBufferCallback), &_newAudioBufferHandlerData);
  gst_caps_unref (audioCaps);
  g_free (audioCapsText);

  // Configure video appsink.
//  GstCaps *videoCaps = gst_caps_from_string ("video/x-raw-rgb");
  GstCaps *videoCaps = gst_caps_from_string ("video/x-raw-rgb,format=RGBA,bpp=32,depth=32");
  g_object_set (_videoSink, "emit-signals", TRUE,
                            "caps", videoCaps,    // this sets video caps to "video/x-raw-rgb"
                            "max-buffers", 1,     // only one buffer (the last) is maintained in the queue
                            "drop", TRUE,         // ... other buffers are dropped
                            NULL);
  g_signal_connect (_videoSink, "new-buffer", G_CALLBACK (Gear_VideoSource::gstNewBufferCallback), &_videoNewBufferCounter);
  gst_caps_unref (videoCaps);

  // Listen to the bus.
  _bus = gst_element_get_bus (_pipeline);

  // Start playing.
  if (!_setPlayState(true))
    return false;

  std::cout << "Pipeline started." << std::endl;

  //_movieReady = true;
	return true;
}


void Gear_VideoSource::runVideo() {

  if (!_VIDEO_OUT->connected())
    return;

  if (!_preRun())
    return;

  if (_videoNewBufferCounter > 0) {

    // Pull video.
    if (!_videoPull())
    {
      _FINISH_OUT->type()->setValue(1.0f);
      _VIDEO_OUT->sleeping(true);
    } else
      _VIDEO_OUT->sleeping(false);


    _videoNewBufferCounter--;
  }

  _postRun();
}


void Gear_VideoSource::runAudio() {

  if (!_AUDIO_OUT->connected())
    return;

  if (!_preRun())
    return;

  unsigned int blockByteSize = Engine::signalInfo().blockSize()*sizeof(Signal_T);
  if (gst_adapter_available(_audioBufferAdapter) >= blockByteSize )
  {
    // Copy block of data to audio output.
    gst_adapter_copy(_audioBufferAdapter, (guint8*)_AUDIO_OUT->type()->data(), 0, blockByteSize);
    gst_adapter_flush (_audioBufferAdapter, blockByteSize);

    _AUDIO_OUT->sleeping(false);
  }
  else
  {
    _FINISH_OUT->type()->setValue(1.0f);
    _AUDIO_OUT->sleeping(true);
  }

  _postRun();
}

bool Gear_VideoSource::_preRun()
{
  // Check for end-of-stream or terminate.
  if (_eos() || _terminate)
  {
    _FINISH_OUT->type()->setValue(1.0f);
    _VIDEO_OUT->sleeping(true);
    _AUDIO_OUT->sleeping(true);

    if (_audioBufferAdapter != NULL)
      gst_adapter_clear(_audioBufferAdapter);
  }
  else
    _FINISH_OUT->type()->setValue(0.0f);

  if (_RESET_IN->type()->boolValue())
    resetMovie();

  if (_currentMovie != _MOVIE_IN->type()->value()) {
    _currentMovie = _MOVIE_IN->type()->value();
    if (!loadMovie(_currentMovie))
      return false;
  }

  if (!_movieReady ||
      !_padHandlerData.isConnected())
    return false;

  return true;
}

void Gear_VideoSource::_postRun()
{
  // Parse message.
  if (_bus != NULL)
  {
    GstMessage *msg = gst_bus_timed_pop_filtered(
                        _bus, 0,
                        (GstMessageType) (GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

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
//        _finish();
        break;

      case GST_MESSAGE_EOS:
        g_print("End-Of-Stream reached.\n");
//        _terminate = true;
//        _finish();
        break;

      case GST_MESSAGE_STATE_CHANGED:
        // We are only interested in state-changed messages from the pipeline.
        if (GST_MESSAGE_SRC (msg) == GST_OBJECT (_pipeline)) {
          GstState oldState, newState, pendingState;
          gst_message_parse_state_changed(msg, &oldState, &newState,
              &pendingState);
          g_print("Pipeline state for movie %s changed from %s to %s:\n",
              _currentMovie.ascii(),
              gst_element_state_get_name(oldState),
              gst_element_state_get_name(newState));

          if (oldState == GST_STATE_PAUSED && newState == GST_STATE_READY)
            gst_adapter_clear(_audioBufferAdapter);

          if (newState == GST_STATE_PLAYING) {
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
        // We should not reach here.
        g_printerr("Unexpected message received.\n");
        break;
      }
      gst_message_unref(msg);
    }
  }
}


bool Gear_VideoSource::_setPlayState(bool play)
{
  if (_pipeline == NULL)
    return false;

  GstStateChangeReturn ret = gst_element_set_state (_pipeline, (play ? GST_STATE_PLAYING : GST_STATE_PAUSED));
  if (ret == GST_STATE_CHANGE_FAILURE)
  {
    std::cout << "Unable to set the pipeline to the playing state." << std::endl;
    unloadMovie();
    return false;
  }
  else
  {
    _setReady(play);

    return true;
  }
}

void Gear_VideoSource::_setReady(bool ready)
{
  _movieReady = ready;
  _VIDEO_OUT->sleeping(!ready);
  _AUDIO_OUT->sleeping(!ready);
}

void Gear_VideoSource::gstPadAddedCallback(GstElement *src, GstPad *newPad, Gear_VideoSource::GstPadHandlerData* data) {
  g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (newPad), GST_ELEMENT_NAME (src));
  bool isAudio = false;
  GstPad *sinkPad = NULL;

  // Check the new pad's type.
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

  // If our converter is already linked, we have nothing to do here.
  if (gst_pad_is_linked (sinkPad))
  {
    // Best prefixes.
    if (g_str_has_prefix (newPadType, "audio/x-raw-float") ||
        g_str_has_prefix (newPadType, "video/x-raw-int") )
    {
      g_print ("  Found a better pad.\n");
      GstPad* oldPad = gst_pad_get_peer(sinkPad);
      gst_pad_unlink(oldPad, sinkPad);
      g_object_unref(oldPad);
    }
    else
    {
      g_print ("  We are already linked. Ignoring.\n");
      goto exit;
    }
  }

  // Attempt the link
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
  // Unreference the new pad's caps, if we got them.
  if (newPadCaps != NULL)
    gst_caps_unref (newPadCaps);

  // Unreference the sink pad.
  if (sinkPad != NULL)
    gst_object_unref (sinkPad);
}

void Gear_VideoSource::gstNewAudioBufferCallback(GstElement *sink, GstNewAudioBufferHandlerData *data) {
  GstBuffer *buffer = NULL;

  // Retrieve the buffer.
  // TODO: we should pull ALL buffers and add them to the adapter
  g_signal_emit_by_name (data->audioSink, "pull-buffer", &buffer);

  if (buffer)
  {
    ASSERT_WARNING_MESSAGE( ! GST_BUFFER_IS_DISCONT(buffer), "Discontinuity detected in audio buffer." );

//    int blockSize  = 2;
//    int sampleRate = 1;
//    int channels  = 0;
//    int width = 0;
//    GstCaps* caps = GST_BUFFER_CAPS(buffer);
//    GstStructure *capsStruct = gst_caps_get_structure (caps, 0);
//
//    gst_structure_get_int(capsStruct, "rate",  &sampleRate);
//    gst_structure_get_int(capsStruct, "channels", &channels);
//    gst_structure_get_int(capsStruct, "width",  &width);

//    std::cout << "rate = " << sampleRate << " channels = " << channels << " width = " << width << std::endl;
//    unsigned int blockByteSize = Engine::signalInfo().blockSize() * sizeof(Signal_T);

//    std::cout << "bufsize: "<< GST_BUFFER_SIZE(buffer) <<
//                 " / adaptersize: " << gst_adapter_available(data->audioBufferAdapter) << std::endl;

    // Add buffer to the adapter.
    gst_adapter_push(data->audioBufferAdapter, buffer);
 //   std::cout << " .. after push = : "<< gst_adapter_available(_audioBufferAdapter);

    // NOTE: no need to unref the buffer here because the buffer was given away with the
    // call to gst_adapter_push()
    //gst_buffer_unref (buffer);
  }
}

void Gear_VideoSource::internalPrePlay()
{
  // Start/resume playback.
  _setPlayState(true);
}

void Gear_VideoSource::internalPostPlay()
{
  // Pause playback.
  _setPlayState(false);
}




