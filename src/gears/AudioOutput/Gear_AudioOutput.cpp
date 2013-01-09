/* Gear_AudioOutput.cpp
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

#include <iostream>
#include "Gear_AudioOutput.h"
#include "GearMaker.h"
#include "Engine.h"

#include "ThreadUtil.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AudioOutput(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AudioOutput";
  gearInfo.classification = GearClassifications::signal().IO().instance();
  return gearInfo;
}
}

const int Gear_AudioOutput::DEFAULT_FRAMES_PER_BUFFER=512;
const int Gear_AudioOutput::DEFAULT_NB_BUFFERS=0;

const std::string Gear_AudioOutput::SETTING_FRAMES_PER_BUFFER = "FramesPerBuffer";
const std::string Gear_AudioOutput::SETTING_NB_BUFFERS = "NbBuffers";

Gear_AudioOutput::Gear_AudioOutput(Schema *schema, std::string uniqueName) : 
Gear(schema, "AudioOutput", uniqueName),
_bus(NULL),
_pipeline(NULL),
_audioSource(NULL),
_audioSink(NULL)
{
  addPlug(_AUDIO_IN_LEFT = new PlugIn<SignalType>(this, "Left", false, new SignalType(0.0f)));    
  addPlug(_AUDIO_IN_RIGHT = new PlugIn<SignalType>(this, "Right", false, new SignalType(0.0f)));    

  std::vector<AbstractPlug*> atLeastOneOfThem;
  atLeastOneOfThem.push_back(_AUDIO_IN_LEFT);
  atLeastOneOfThem.push_back(_AUDIO_IN_RIGHT);
  setPlugAtLeastOneNeeded(atLeastOneOfThem);

  _settings.add(Property::INT, SETTING_FRAMES_PER_BUFFER)->valueInt(DEFAULT_FRAMES_PER_BUFFER);
  _settings.add(Property::INT, SETTING_NB_BUFFERS)->valueInt(DEFAULT_NB_BUFFERS);
}

Gear_AudioOutput::~Gear_AudioOutput()
{
}

bool Gear_AudioOutput::initOutput()
{
  std::cout << "Opening audio output device" << std::endl;

  //free previously allocated structures
  freeResources();

  GstStateChangeReturn ret;

  // Initialize GStreamer.
  gst_init (NULL, NULL);

  // Create the elements.
  _audioSource = gst_element_factory_make ("appsrc", "asource");
  _audioSink =   gst_element_factory_make ("autoaudiosink", "asink");

//  _feedData = GstFeedData();
//  _feedData.audioSource = _audioSource;
//  _feedData.plug        = _AUDIO_IN_LEFT;

  _needsData = false;

  // Create the empty pipeline.
  _pipeline = gst_pipeline_new ( "audio-output-pipeline" );

  if (!_pipeline || !_audioSource || !_audioSink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  // Build the pipeline. Note that we are NOT linking the source at this
  // point. We will do it later.
  gst_bin_add_many (GST_BIN (_pipeline), _audioSource, _audioSink, NULL);

  // Connect source to sink.
  if (!gst_element_link (_audioSource, _audioSink)) {
    g_printerr ("Video elements could not be linked.\n");
    freeResources();
    //gst_object_unref (_pipeline);
    return false;
  }

  // Configure video appsink.
  gchar* audioCapsText = g_strdup_printf ("audio/x-raw-float,channels=1,rate=%d,signed=(boolean)true,width=%d,depth=%d,endianness=BYTE_ORDER",
                                          Engine::signalInfo().sampleRate(), (int)(sizeof(Signal_T)*8), (int)(sizeof(Signal_T)*8) );
  GstCaps* audioCaps = gst_caps_from_string (audioCapsText);
  g_object_set (_audioSource, "caps", audioCaps, NULL);
  g_signal_connect (_audioSource, "need-data",   G_CALLBACK (Gear_AudioOutput::gstNeedsDataCallback), this);
  g_signal_connect (_audioSource, "enough-data", G_CALLBACK (Gear_AudioOutput::gstEnoughDataCallback), this);
  gst_caps_unref (audioCaps);
  g_free (audioCapsText);

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
//
//  //  _audioHasNewBuffer = false;
//  _videoHasNewBuffer = false;
//
//  _cameraReady=true;
//  _VIDEO_OUT->sleeping(false);
//
//  _terminate = false;

  _testIter = 0;

  return true;
}

void Gear_AudioOutput::freeResources()
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
  //_cameraReady=false;
  //_VIDEO_OUT->sleeping(true);

  // Unsynch.
  unSynch();
}

void Gear_AudioOutput::internalInit()
{
  initOutput();
}

void Gear_AudioOutput::onUpdateSettings()
{
//  init();
}

void Gear_AudioOutput::runAudio()
{	
  if (_needsData)
  {
    GstBuffer *buffer;
    GstFlowReturn ret;

    /* Create a new empty buffer */
    const SignalType* audio = _AUDIO_IN_LEFT->type();
    unsigned int blockByteSize = audio->size() * sizeof (Signal_T);
    //unsigned int sampleRate = Engine::signalInfo().sampleRate();
    //unsigned int blockSize  = Engine::signalInfo().blockSize();
    buffer = gst_buffer_new_and_alloc ( blockByteSize );

    /* Set its timestamp and duration */
//    GST_BUFFER_TIMESTAMP (buffer) = gst_util_uint64_scale (_testIter * blockSize, GST_SECOND, sampleRate);
//    GST_BUFFER_DURATION  (buffer) = gst_util_uint64_scale (blockSize, GST_SECOND, sampleRate);

    //std::cout << GST_BUFFER_DURATION (buffer) << " / " << GST_BUFFER_TIMESTAMP (buffer) << " bs: " << blockByteSize << " sr: " << sampleRate << std::endl;
    /* Generate some psychodelic waveforms */
    memcpy(GST_BUFFER_DATA(buffer), audio->data(), blockByteSize);

    /* Push the buffer into the appsrc */
    g_signal_emit_by_name (_audioSource, "push-buffer", buffer, &ret);

    /* Free the buffer now that we are done with it */
    gst_buffer_unref (buffer);

    if (ret != GST_FLOW_OK) {
      /* We got some error, stop sending data */
      std::cout << "We got some error, stop sending data" << std::endl;
      exit(-1); // XXX baaaad!
  //    return false;
    }
  }
  else
    std::cout << "Enough data" << std::endl;

  _testIter++;

  /*
  const float *left_buffer  = _AUDIO_IN_LEFT->type()->data();
  int signal_blocksize = Engine::signalInfo().blockSize();
  
  for (int i=0; i<signal_blocksize; i++)
    _lBuffer[_readIndex++] = left_buffer[i];

  _readIndex %= _ringBufferSize;    */

}
//
//void Gear_AudioOutput::gstStartFeedCallback(GstElement *source, guint size, GstFeedData *data)
//{
//  std::cout << "Need data" << std::endl;
//
//  return;
//
//  GstBuffer *buffer;
//  GstFlowReturn ret;
//  int i;
////
////  gint16 *raw;
////  gint num_samples = CHUNK_SIZE / 2; /* Because each sample is 16 bits */
////  gfloat freq;
//
//  /* Create a new empty buffer */
//  const SignalType* audio = data->plug->type();
//  unsigned int blockByteSize = audio->size() * sizeof (Signal_T);
//  unsigned int sampleRate = Engine::signalInfo().sampleRate();
//  buffer = gst_buffer_new_and_alloc ( blockByteSize );
//
//  /* Set its timestamp and duration */
// // GST_BUFFER_TIMESTAMP (buffer) = gst_util_uint64_scale (data->num_samples, GST_SECOND, sampleRate);
// // GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale (blockByteSize, GST_SECOND, sampleRate);
//
//  /* Generate some psychodelic waveforms */
//  memcpy(GST_BUFFER_DATA(buffer), audio->data(), blockByteSize);
//
//  /* Push the buffer into the appsrc */
//  g_signal_emit_by_name (data->audioSource, "push-buffer", buffer, &ret);
//
//  /* Free the buffer now that we are done with it */
//  gst_buffer_unref (buffer);
//
//  if (ret != GST_FLOW_OK) {
//    /* We got some error, stop sending data */
//    std::cout << "We got some error, stop sending data" << std::endl;
//    exit(-1); // XXX baaaad!
////    return false;
//  }
//
////  return true;
//}
//
//void Gear_AudioOutput::gstStopFeedCallback(GstElement *source, guint size, GstFeedData *data)
//{
//
//}

void Gear_AudioOutput::gstNeedsDataCallback(GstElement *source, guint size, Gear_AudioOutput *gear)
{
  gear->needsData(true);
}

void Gear_AudioOutput::gstEnoughDataCallback(GstElement *source, guint size, Gear_AudioOutput *gear)
{
  gear->needsData(false);
}

void Gear_AudioOutput::internalPrePlay()
{
//   Pa_StartStream(_stream);
}    

void Gear_AudioOutput::internalPostPlay()
{
//   Pa_AbortStream(_stream);
}    


//void Gear_AudioOutput::initPortAudio()
//{
//
//  std::cout << "Init PortAudio stream..." << std::endl;
//
//  if (_stream)
//  {
//    Pa_AbortStream(_stream);
//    Pa_CloseStream(_stream);
//  }
//
//  int framesPerBuffer = _settings.get(SETTING_FRAMES_PER_BUFFER)->valueInt();
//  _ringBufferSize = framesPerBuffer;
//
//  std::cout << "Opening PortAudio Stream..." << std::endl;
//  std::cout << Engine::signalInfo().sampleRate() << "hz " << std::endl;
//  std::cout << "Frames per buffer: " << framesPerBuffer << std::endl;
//
//  int nbBuffers = _settings.get(SETTING_NB_BUFFERS)->valueInt();
//  //if nbBuffers is 0, let portaudio set the value
//  if (!nbBuffers)
//  {
//      nbBuffers = Pa_GetMinNumBuffers(framesPerBuffer, Engine::signalInfo().sampleRate());
//      _settings.get(SETTING_NB_BUFFERS)->valueInt(nbBuffers);
//  }
//
//  _lBuffer.resize(_ringBufferSize);
//
//  std::cout << "Default output device: " <<  Pa_GetDefaultOutputDeviceID() << std::endl;
//  std::cout << "Number of different devices: " << Pa_CountDevices() << std::endl;
//  PaError err = Pa_OpenStream(&_stream,
//                             paNoDevice,//no input
//                             0,
//                             paFloat32,
//                             NULL,
//                             Pa_GetDefaultOutputDeviceID(),//default output device
//                             1,          //mono
//                             paFloat32,
//                             NULL,
//                             Engine::signalInfo().sampleRate(),
//                             framesPerBuffer,
//                             nbBuffers,
//                             paClipOff,      /* we won't output out of range samples so don't bother clipping them*/
//                             portAudioCallback,
//                             this);
//
//
//  if (err != paNoError)
//    std::cout << Pa_GetErrorText(err) << std::endl;
//  else
//    std::cout << "Opening PortAudio Stream...done" << std::endl;
//}
//
//int Gear_AudioOutput::portAudioCallback(void *, void *output_buffer, unsigned long frames_per_buffer,
//                                        PaTimestamp, void *user_data)
//{
//  Gear_AudioOutput *parent = (Gear_AudioOutput*)user_data;
//
//  ScopedLock scopedLock(parent->_mutex);
//
//  SignalType& lbuffer = parent->_lBuffer;
//  int& lindex = parent->_lBufferIndex;
//
//  float *out = (float*)output_buffer;
//
//  for (unsigned int i=0; i<frames_per_buffer; i++ )
//    *out++ = lbuffer[lindex++];
//
//  lindex %= parent->_ringBufferSize;
//
//  return 0;
//}
//

