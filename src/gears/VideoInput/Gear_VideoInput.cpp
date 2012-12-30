/* Gear_VideoInput.cpp
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
//inspired from Using libavformat and libavcodec by Martin B�hme (boehme@inb.uni-luebeckREMOVETHIS.de) 


#include <iostream>
#include "Gear_VideoInput.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_VideoInput(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "VideoInput";
    gearInfo.classification = GearClassifications::video().IO().instance();
    return gearInfo;
  }
}

bool Gear_VideoInput::_videoPull()
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

bool Gear_VideoInput::_eos() const
{
  if (_cameraReady && _videoSink) {
    gboolean eos;
    g_object_get (G_OBJECT (_videoSink), "eos", &eos, NULL);
    return (bool)eos;
  }
  else
    return false;
}


void Gear_VideoInput::gstNewBufferCallback(GstElement*, bool *newBuffer)
{
  *newBuffer = true;
}

Gear_VideoInput::Gear_VideoInput(Schema *schema, std::string uniqueName) : 
Gear(schema, "VideoInput", uniqueName),
_currentMovie(""),
_bus(NULL),
_pipeline(NULL),
_source(NULL),
_videoQueue(NULL),
_videoColorSpace(NULL),
_videoSink(NULL),
//_audioHasNewBuffer(false),
_videoHasNewBuffer(false),
_cameraReady(false)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut", false));

  _VIDEO_OUT->sleeping(true);
}

Gear_VideoInput::~Gear_VideoInput()
{
  freeResources();
}

void Gear_VideoInput::internalPrePlay()
{
  if (!initCamera())
  {
    std::cout << "Could not initialize capture device. Verify your video capture device by running gstreamer-properties." << std::endl;
  }
}

bool Gear_VideoInput::initCamera()
{
  std::cout << "Opening video capture device" << std::endl;

  //free previously allocated structures
  freeResources();

  GstStateChangeReturn ret;

  // Initialize GStreamer.
  gst_init (NULL, NULL);

  // Create the elements.
  _source =          gst_element_factory_make ("autovideosrc", "source");

//  _videoQueue =      gst_element_factory_make ("queue", "vqueue");
  _videoColorSpace = gst_element_factory_make ("ffmpegcolorspace", "vcolorspace");
  _videoSink =       gst_element_factory_make ("appsink", "vsink");

  // Create the empty pipeline.
  _pipeline = gst_pipeline_new ( "video-input-pipeline" );

  if (!_pipeline || !_source ||
      /*!_videoQueue ||*/ !_videoColorSpace || !_videoSink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  // Build the pipeline. Note that we are NOT linking the source at this
  // point. We will do it later.
  gst_bin_add_many (GST_BIN (_pipeline), _source,
                    /*_videoQueue,*/ _videoColorSpace, _videoSink, NULL);

  if (/*!gst_element_link (_source, _videoQueue) ||
      !gst_element_link (_videoQueue, _videoColorSpace) ||*/
      !gst_element_link (_source, _videoColorSpace) ||
      !gst_element_link (_videoColorSpace, _videoSink)) {
    g_printerr ("Video elements could not be linked.\n");
    gst_object_unref (_pipeline);
    return false;
  }

  // Configure video appsink.
  GstCaps *videoCaps = gst_caps_from_string ("video/x-raw-rgb");
  g_object_set (_videoSink, "emit-signals", TRUE,
                            "caps", videoCaps,    // this sets video caps to "video/x-raw-rgb"
                            "max-buffers", 1,     // only one buffer (the last) is maintained in the queue
                            "drop", true, NULL);  // ... other buffers are dropped
  g_signal_connect (_videoSink, "new-buffer", G_CALLBACK (Gear_VideoInput::gstNewBufferCallback), &_videoHasNewBuffer);
  gst_caps_unref (videoCaps);

  // Start playing.
  ret = gst_element_set_state (_pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    std::cout << "Unable to set the pipeline to the playing state." << std::endl;
    gst_object_unref (_pipeline);
    return false;
  }

  // Listen to the bus.
  _bus = gst_element_get_bus (_pipeline);

  //  _audioHasNewBuffer = false;
  _videoHasNewBuffer = false;

  _cameraReady=true;
  _VIDEO_OUT->sleeping(false);

  _terminate = false;
}

void Gear_VideoInput::internalPostPlay()
{
  freeResources();
}

void Gear_VideoInput::freeResources()
{
  // Free resources.
  if (_bus)
    gst_object_unref (_bus);

  if (_pipeline)
  {
    gst_element_set_state (_pipeline, GST_STATE_NULL);
    gst_object_unref (_pipeline);
  }

  // Init.
  _cameraReady=false;
	_VIDEO_OUT->sleeping(true);

	// Unsynch.
	unSynch();
}

void Gear_VideoInput::runVideo() {

  if (_eos())
    return;

  if (!_cameraReady)
    return;

  if (_terminate) {
//    _FINISH_OUT->type()->setValue(1.0f);
    return;
  }

//  _FINISH_OUT->type()->setValue(0.0f);

  GstMessage *msg = gst_bus_timed_pop_filtered(
                      _bus, 0,
                      (GstMessageType) (GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

  if (_videoHasNewBuffer) {

    // Pull video.
    if (!_videoPull())
      std::cout << "Cannot grab image from device" << std::endl;
//      _FINISH_OUT->type()->setValue(1.0f);

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
      //_FINISH_OUT->type()->setValue(1.0f);
      break;
    case GST_MESSAGE_EOS:
      g_print("End-Of-Stream reached.\n");
      _terminate = true;
      //_FINISH_OUT->type()->setValue(1.0f);
      break;
    case GST_MESSAGE_STATE_CHANGED:
      // We are only interested in state-changed messages from the pipeline.
      if (GST_MESSAGE_SRC (msg) == GST_OBJECT (_pipeline)) {
        GstState oldState, newState, pendingState;
        gst_message_parse_state_changed(msg, &oldState, &newState,
            &pendingState);
        g_print("Pipeline state changed from %s to %s:\n",
            gst_element_state_get_name(oldState),
            gst_element_state_get_name(newState));
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



