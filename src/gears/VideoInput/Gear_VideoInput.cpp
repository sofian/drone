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


extern "C" {
  Gear* makeGear()
  {
    return new Gear_VideoInput();
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

    memcpy(video->data(), GST_BUFFER_DATA(buffer), video->size() * sizeof(RGBA));

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

Gear_VideoInput::Gear_VideoInput() :
Gear("VideoInput"),
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
    freeResources();
    //gst_object_unref (_pipeline);
    return false;
  }

  // Configure video appsink.
  GstCaps *videoCaps = gst_caps_from_string ("video/x-raw-rgb,format=RGBA,bpp=32,depth=32");
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
    freeResources();
    // gst_object_unref (_pipeline);
    return false;
  }

  // Listen to the bus.
  _bus = gst_element_get_bus (_pipeline);

  //  _audioHasNewBuffer = false;
  _videoHasNewBuffer = false;

  _cameraReady=true;
  _VIDEO_OUT->sleeping(false);

  _terminate = false;

  return true;
}

void Gear_VideoInput::internalPrePlay()
{
  if (!initCamera())
  {
    std::cout << "Could not initialize capture device. Verify your video capture device by running gstreamer-properties." << std::endl;
  }
}

void Gear_VideoInput::internalPostPlay()
{
  freeResources();
}

void Gear_VideoInput::freeResources()
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

  if (_videoHasNewBuffer) {

    // Pull video.
    if (!_videoPull())
      std::cout << "Cannot grab image from device" << std::endl;
//      _FINISH_OUT->type()->setValue(1.0f);

    _videoHasNewBuffer = false;
  }

  if (_bus != NULL)
  {
    GstMessage *msg = gst_bus_timed_pop_filtered(
                        _bus, 0,
                        (GstMessageType) (GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

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
}










/* void Gear_VideoInput::resetInputDevice()                                                                                   */
/* {                                                                                                                          */
/*   if (_device!=0)                                                                                                          */
/*     close(_device);                                                                                                        */
/*                                                                                                                            */
/*   _sizeX=0;                                                                                                                */
/*   _sizeY=0;                                                                                                                */
/*                                                                                                                            */
/*     if (_bufferBGRA!=NULL)                                       */
/*     {                                                            */
/*          munmap(_bufferBGRA, _sizeX*_sizeY*VIDEO_PALETTE_RGB32); */
/*         _bufferBGRA=NULL;                                        */
/*     }                                                            */
/*                                                                  */
/*     memset(&_vidCap, 0, sizeof(video_capability));               */
/*     memset(&_vidWin, 0, sizeof(video_window));                   */
/*     memset(&_vidPic, 0, sizeof(video_picture));                  */
/* }                                                                                                                          */
/*                                                                                                                            */
/*                                                                                                                            */
/* void Gear_VideoInput::prePlay()                                                                                            */
/* {                                                                                                                          */
/*   //lavrec_main(_lavrecInfo);                                                                                              */
/*   //lavrec_start(_lavrecInfo);                                                                                             */
/*                                                                                                                            */
/* }                                                                                                                          */
/*                                                                                                                            */
/* void Gear_VideoInput::videoCallback(uint8_t *video, long size, long count) */
/* {                                                                          */
/*   std::cout << size << std::endl;                                          */
/*                                                                            */
/*                                                                            */
/*   unsigned char *data1 = (unsigned char*)malloc(160*120*4);                */
/*   unsigned char *data2 = (unsigned char*)malloc(160*120*4);                */
/*   decode_jpeg_raw(video, size, 0, 420, 160, 120, _data, data1, data2);     */
/*                                                                            */
/* }                                                                          */

/* void audioCallback(uint8_t *audio, long sampes) */
/* {                                               */
/*                                                 */
/* }                                               */

/* void Gear_VideoInput::initInputDevice() */
/* {                                       */
/*     _lavrecInfo = lavrec_malloc();               */
/*     _lavrecInfo->video_dev = "/dev/video0";      */
/*     _lavrecInfo->video_captured = videoCallback; */
/*     _lavrecInfo->audio_captured = audioCallback; */
/*     _lavrecInfo->use_read = 1;                   */
/*     _lavrecInfo->video_norm = 1;                 */
/*     rect *rec=(rect*)malloc(sizeof(rect));       */
/*     rec->x=0;                                    */
/*     rec->y=0;                                    */
/*     rec->w=640;                                  */
/*     rec->h=480;                                  */
/*     //_lavrecInfo->geometry = rec;               */
/*     _lavrecInfo->quality = 100;                  */
/*     _lavrecInfo->video_format = 'j';             */
/*                                                                                                                            */
/*                                                                                                                            */
/*   resetInputDevice();                                                                                                      */
/*                                                                                                                            */
/*   _device = open(_settings.get(SETTING_DEVICE)->valueStr().c_str(), O_RDWR);                                               */
/*                                                                                                                            */
/*   if (_device<=0)                                                                                                          */
/*   {                                                                                                                        */
/*     std::cout << "fail to open device " << _settings.get(SETTING_DEVICE)->valueStr().c_str() << std::endl;                 */
/*     return;                                                                                                                */
/*   }                                                                                                                        */
/*                                                                                                                            */
/*   //get info                                                                                                               */
/*   ioctl(_device, VIDIOCGCAP, &_vidCap);                                                                                    */
/*   ioctl(_device, VIDIOCGWIN, &_vidWin);                                                                                    */
/*   _vidWin.width=320;                                                                                                       */
/*   _vidWin.height=200;                                                                                                      */
/*   ioctl(_device, VIDIOCSWIN, &_vidWin);                                                                                    */
/*   ioctl(_device, VIDIOCGPICT, &_vidPic);                                                                                   */
/*   ioctl(_device, VIDIOCGFBUF, &_vidBuf);                                                                                   */
/*   //ioctl(_device, VIDIOCGTUNER, &_vidTuner);                                                                              */
/*   struct video_channel _vidChan;                                                                                           */
/*   _vidChan.channel = 0;                                                                                                    */
/*   ioctl(_device, VIDIOCGCHAN, &_vidChan);                                                                                  */
/*   //std::cout << "channel: " << _vidChan.channel << std::endl << _vidChan.name << std::endl << _vidChan.type << std::endl; */
/*   _vidChan.norm = 1;                                                                                                       */
/*   ioctl(_device, VIDIOCSCHAN, &_vidChan);                                                                                  */
/*                                                                                                                            */
/*                                                                                                                            */
/*                                                                                                                            */
/*   std::cout << "palette : " << _vidPic.palette << std::endl;                                                               */
/*                                                                                                                            */
/*   _vidPic.palette = VIDEO_PALETTE_RGB32;                                                                                   */
/*                                                                                                                            */
/*   ioctl(_device, VIDIOCSPICT, &_vidPic);                                                                                   */
/*                                                                                                                            */
/*                                                                                                                            */
/*   _vidWin.clips = _vidClips;                                                                                               */
/*   _vidWin.clipcount = 0;                                                                                                   */
/*   _sizeX = _vidCap.maxwidth;                                                                                               */
/*   _sizeY = _vidCap.maxheight;                                                                                              */
/*                                                                                                                            */
/*   //show info                                                                                                              */
/*   std::cout << "-- device info --" << std::endl;                                                                           */
/*   std::cout << "name   : " << _vidCap.name << std::endl;                                                                   */
/*   std::cout << "type   : " << _vidCap.type << std::endl;                                                                   */
/*   std::cout << "size X : " << _sizeX << std::endl;                                                                         */
/*   std::cout << "size Y : " << _sizeY << std::endl;                                                                         */
/*   std::cout << "bpp    : " << _vidPic.depth << std::endl;                                                                  */
/*   std::cout << "Overlay : " << ((_vidCap.type & VID_TYPE_OVERLAY) ? "Y" : "N") << std::endl;                               */
/*                                                                                                                            */
/*   struct zoran_params params;                                 */
/*   ioctl(_device, BUZIOC_G_PARAMS, &params);                   */
/*   std::cout << "decimationH: " << params.HorDcm << std::endl; */
/*   std::cout << "decimationV: " << params.VerDcm << std::endl; */
/*   std::cout << "quality: " << params.quality << std::endl;    */
/*   std::cout << "xres: " << params.img_width << std::endl;     */
/*   std::cout << "yres: " << params.img_height << std::endl;    */
/*                                                               */
/*   params.decimation=2;                                        */
/*   ioctl(_device, BUZIOC_S_PARAMS, &params);                   */

/*     _vidBuf.width = _sizeX;                */
/*     _vidBuf.height = _sizeY;               */
/*     ioctl(_device, VIDIOCSFBUF, &_vidBuf); */
/*                                                                                                                            */
/*   int temp=0;                                                                                                              */
/*   ioctl(_device, VIDIOCCAPTURE, &temp);                                                                                    */
/*                                                                                                                            */
/*   //prepare and allocate mmap                                                                                              */
/*   if (ioctl(_device, VIDIOCGMBUF, &_vidMBuf)==-1)                                                      */
/*   {                                                                                                    */
/*     std::cout << "fdgdfgfd" << std::endl;                                                              */
/*     perror("VIDIOCGMBUF");                                                                             */
/*   }                                                                                                    */
/*   std::cout << "buffer size : " << _vidMBuf.size << std::endl;                                         */
/*                                                                                                        */
/*   _bufferBGRA = (unsigned char*) mmap(0, _vidMBuf.size, PROT_READ|PROT_WRITE, MAP_SHARED, _device, 0); */
/*                                                                                                        */
/*   if ((int)_bufferBGRA < 0)                                                                            */
/*   {                                                                                                    */
/*     std::cout << "fail to create mmap!" << std::endl;                                                  */
/*     _bufferBGRA=NULL;                                                                                  */
/*     return;                                                                                            */
/*   }                                                                                                    */
/*                                                                                                        */
/*   _vidMMap.format = VIDEO_PALETTE_RGB32;                                                               */
/*   _vidMMap.frame  = 0;                                                                                 */
/*   _vidMMap.width  = _sizeX;                                                                            */
/*   _vidMMap.height = _sizeY;                                                                            */
/*                                                                                                                            */
/*   _VIDEO_OUT->type()->image()->resize(_sizeY, _sizeX);                                                                     */
/*   _data = (unsigned char*)malloc(_sizeX*_sizeY*4);                                                                         */
/* }                                                                                                                          */
/*                                                                                                                            */
/* void Gear_VideoInput::runVideo()                                                                                           */
/* {                                                                                                                          */
/*   _outData = (unsigned char*) _VIDEO_OUT->type()->image()->data();                                                         */
/*                                                                                                                            */
/*   //read(_device, _data, _sizeX*_sizeY*4);             */
/*   if (ioctl(_device, VIDIOCMCAPTURE, &(_vidMMap))<0)   */
/*   {                                                    */
/*     perror("VIDIOCMCAPTURE");                          */
/*     //return;                                          */
/*   }                                                    */
/*                                                        */
/*   if (ioctl(_device, VIDIOCSYNC, &(_vidMMap.frame))<0) */
/*   {                                                    */
/*     perror("VIDIOCSYNC");                              */
/*     //return(-1);                                      */
/*   }                                                    */
/*                                                        */
/*                                                        */
/*   //convert BGRA -> RGBA                               */
/*   //need optimisation                                  */
/*   int imgSize=_sizeX*_sizeY;                           */
/*   _tempOutData = _outData;                             */
/*   _tempInData =  _bufferBGRA;                          */
/*   for (int i=0;i<imgSize;i++)                          */
/*   {                                                    */
/*     *(_tempOutData) = *(_tempInData);                  */
/*     *(_tempOutData+1) = *(_tempInData+1);              */
/*     *(_tempOutData+2) = *(_tempInData+2);              */
/*                                                        */
/*     _tempOutData+=4;                                   */
/*     _tempInData+=4;                                    */
/*   }                                                    */
/*                                                                                                                            */
/*                                                                                                                            */
/*   int len=0;                                                                                                               */
/*                                                                                                                            */
/*   len = read(_device, _data, _sizeX * _sizeY * 4);                                                                         */
/*                                                                                                                            */
/*   //convert BGRA -> RGBA                                                                                                   */
/*   //need optimisation                                                                                                      */
/*   if (len)                                                                                                                 */
/*   {                                                                                                                        */
/*       int imgSize=_sizeX*_sizeY;                                                                                           */
/*       _tempOutData = _outData;                                                                                             */
/*       _tempInData = _data;                                                                                                 */
/*       for (int i=0;i<imgSize;i++)                                                                                          */
/*       {                                                                                                                    */
/*           *(_tempOutData) = *(_tempInData+2);                                                                              */
/*           *(_tempOutData+1) = *(_tempInData+1);                                                                            */
/*           *(_tempOutData+2) = *(_tempInData);                                                                              */
/*                                                                                                                            */
/*           _tempOutData+=4;                                                                                                 */
/*           _tempInData+=4;                                                                                                  */
/*       }                                                                                                                    */
/*                                                                                                                            */
/*   }                                                                                                                        */
/*                                                                                                                            */
/*                                                                                                                            */
/* }                                                                                                                          */
/*                                                                                                                            */
/*                                                                                                                            */

