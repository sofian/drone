/* Gear_AudioInput.cpp
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
#include "Gear_AudioInput.h"
#include "GearMaker.h"
#include "Engine.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AudioInput(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AudioInput";
  gearInfo.classification = GearClassifications::signal().IO().instance();

  return gearInfo;
}
}

const int Gear_AudioInput::DEFAULT_FRAMES_PER_BUFFER=512;
const int Gear_AudioInput::DEFAULT_NB_BUFFERS=0;//auto

const std::string Gear_AudioInput::SETTING_FRAMES_PER_BUFFER = "FramesPerBuffer";
const std::string Gear_AudioInput::SETTING_NB_BUFFERS = "NbBuffers";

Gear_AudioInput::Gear_AudioInput(Schema *schema, std::string uniqueName) : 
  Gear(schema, "AudioInput", uniqueName),
  _stream(0),
  _ringBufferSize(512),
  _lBuffer(),
  _rBuffer(),
  _lBufferIndex(0),
  _readIndex(0)
{
  //  _category << Category::AUDIO << Category::IO;

  addPlug(_AUDIO_OUT_LEFT = new PlugOut<SignalType>(this, "Left"));    
  addPlug(_AUDIO_OUT_RIGHT = new PlugOut<SignalType>(this, "Right"));    

  _settings.add(Property::INT, SETTING_FRAMES_PER_BUFFER)->valueInt(DEFAULT_FRAMES_PER_BUFFER);
  _settings.add(Property::INT, SETTING_NB_BUFFERS)->valueInt(DEFAULT_NB_BUFFERS);    

  std::cout << "init PortAudio..." << std::endl;

  PaError err = Pa_Initialize();

  if (err != paNoError)
    std::cout << Pa_GetErrorText(err) << std::endl;
  else
    std::cout << "init PortAudio...done" << std::endl;

}

Gear_AudioInput::~Gear_AudioInput()
{   
  if (_stream)
  {  
    Pa_AbortStream(_stream);
    Pa_CloseStream(_stream);
  }
  Pa_Terminate();
}

bool Gear_AudioInput::ready()
{
  return(_AUDIO_OUT_LEFT->connected() || _AUDIO_OUT_RIGHT->connected());
}

void Gear_AudioInput::init()
{
  std::cout << "Initializing AudioInput..." << std::endl;

  initPortAudio();
  
  std::cout << "Initializing AudioInput...done" << std::endl;
}

void Gear_AudioInput::onUpdateSettings()
{
  init();
}

void Gear_AudioInput::runAudio()
{
  float *left_buffer = _AUDIO_OUT_LEFT->type()->data();
  //SignalType right_buffer  = _AUDIO_OUT_RIGHT->type();
  int signal_blocksize = Engine::signalInfo().blockSize();

  for (int i=0; i<signal_blocksize; i++)
    left_buffer[i] = _lBuffer[_readIndex++];

  _readIndex %= _ringBufferSize;    

}

void Gear_AudioInput::initPortAudio()
{
  std::cout << "Opening PortAudio Stream..." << std::endl;
  
  if (_stream)
  {  
    Pa_AbortStream(_stream);
    Pa_CloseStream(_stream);
  }
  
  std::cout << Engine::signalInfo().sampleRate() << "hz " << std::endl;
  
  int framesPerBuffer = _settings.get(SETTING_FRAMES_PER_BUFFER)->valueInt();
  std::cout << "Frames per buffer: " << framesPerBuffer << std::endl;

  int nbBuffers = _settings.get(SETTING_NB_BUFFERS)->valueInt();
  //if nbBuffers is 0, let portaudio set the value
  if (!nbBuffers)
  {
      nbBuffers = Pa_GetMinNumBuffers(framesPerBuffer, Engine::signalInfo().sampleRate());
      _settings.get(SETTING_NB_BUFFERS)->valueInt(nbBuffers);
  }
  
  std::cout << "Number of buffers: " << nbBuffers << std::endl;

  _ringBufferSize = framesPerBuffer;

  _lBuffer.resize(_ringBufferSize);

  PaError err = Pa_OpenStream(&_stream,
                             Pa_GetDefaultInputDeviceID(),/* default input device*/
                             1,              //input
                             paFloat32,     
                             NULL,
                             paNoDevice,    //no output
                             0,             //mono          
                             paFloat32,     
                             NULL,
                             Engine::signalInfo().sampleRate(),
                             framesPerBuffer,
                             nbBuffers,              /* number of buffers, if zero then use default minimum*/
                             paClipOff,      /* we won't output out of range samples so don't bother clipping them*/
                             portAudioCallback,
                             this);


  if (err != paNoError)
    std::cout << Pa_GetErrorText(err) << std::endl;
  else
    std::cout << "Opening PortAudio Stream...done" << std::endl;

}

void Gear_AudioInput::prePlay()
{
   Pa_StartStream(_stream);
}    

void Gear_AudioInput::postPlay()
{
   Pa_AbortStream(_stream);
}    


int Gear_AudioInput::portAudioCallback(void *input_buffer, void *, unsigned long frames_per_buffer,
                                       PaTimestamp, void *user_data)
{
  Gear_AudioInput *parent = (Gear_AudioInput*)user_data;
      
  SignalType& lbuffer = parent->_lBuffer;
  int& lindex = parent->_lBufferIndex;
  
  float *in = (float*)input_buffer;

  for (unsigned int i=0; i<frames_per_buffer; i++ )
    lbuffer[lindex++] = *in++;

  lindex %= parent->_ringBufferSize;

  return 0;
}


