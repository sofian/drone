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


Register_Gear(MAKERGear_AudioOutput, Gear_AudioOutput, "AudioOutput")

const int Gear_AudioOutput::DEFAULT_FRAMES_PER_BUFFER=512;
const int Gear_AudioOutput::DEFAULT_NB_BUFFERS=0;

const std::string Gear_AudioOutput::SETTING_FRAMES_PER_BUFFER = "FramesPerBuffer";
const std::string Gear_AudioOutput::SETTING_NB_BUFFERS = "NbBuffers";

Gear_AudioOutput::Gear_AudioOutput(Schema *schema, std::string uniqueName) : 
Gear(schema, "AudioOutput", uniqueName),     
_stream(0),
_ringBufferSize(512),
_lBuffer(),
_rBuffer(),
_lBufferIndex(0),
_readIndex(0)

{
  addPlug(_AUDIO_IN_LEFT = new PlugIn<SignalType>(this, "Left", new SignalType(0.0f)));    
  addPlug(_AUDIO_IN_RIGHT = new PlugIn<SignalType>(this, "Right", new SignalType(0.0f)));    

  _settings.add(Property::INT, SETTING_FRAMES_PER_BUFFER)->valueInt(DEFAULT_FRAMES_PER_BUFFER);
  _settings.add(Property::INT, SETTING_NB_BUFFERS)->valueInt(DEFAULT_NB_BUFFERS);    

  std::cout << "init PortAudio..." << std::endl;

  PaError err = Pa_Initialize();

  if (err != paNoError)
    std::cout << Pa_GetErrorText(err) << std::endl;
  else
    std::cout << "init PortAudio...done" << std::endl;

}

Gear_AudioOutput::~Gear_AudioOutput()
{
  if (_stream)
  {  
    Pa_AbortStream(_stream);
    Pa_CloseStream(_stream);
  }
  Pa_Terminate();

}

bool Gear_AudioOutput::ready()
{
  return(_AUDIO_IN_LEFT->connected() || _AUDIO_IN_RIGHT->connected());
}

void Gear_AudioOutput::init()
{
  std::cout << "Initializing AudioOutput..." << std::endl;

  initPortAudio();

  std::cout << "Initializing AudioOutput...done" << std::endl;
}

void Gear_AudioOutput::onUpdateSettings()
{
  init();
}

void Gear_AudioOutput::runAudio()
{
  const float *left_buffer  = _AUDIO_IN_LEFT->type()->data();
  int signal_blocksize = Engine::signalInfo().blockSize();
  
  for (int i=0; i<signal_blocksize; i++)
    _lBuffer[_readIndex++] = left_buffer[i];

  _readIndex %= _ringBufferSize;    

}

void Gear_AudioOutput::prePlay()
{
   Pa_StartStream(_stream);
}    

void Gear_AudioOutput::postPlay()
{
   Pa_AbortStream(_stream);
}    


void Gear_AudioOutput::initPortAudio()
{

  std::cout << "init PortAudio..." << std::endl;

  if (_stream)
  {  
    Pa_AbortStream(_stream);
    Pa_CloseStream(_stream);
  }

  int framesPerBuffer = _settings.get(SETTING_FRAMES_PER_BUFFER)->valueInt();
  _ringBufferSize = framesPerBuffer;

  std::cout << "Opening PortAudio Stream..." << std::endl;
  std::cout << Engine::signalInfo().sampleRate() << "hz " << std::endl;
  std::cout << "Frames per buffer: " << framesPerBuffer << std::endl;

  int nbBuffers = _settings.get(SETTING_NB_BUFFERS)->valueInt();
  //if nbBuffers is 0, let portaudio set the value
  if (!nbBuffers)
  {
      nbBuffers = Pa_GetMinNumBuffers(framesPerBuffer, Engine::signalInfo().sampleRate());
      _settings.get(SETTING_NB_BUFFERS)->valueInt(nbBuffers);
  }
 
  _lBuffer.resize(_ringBufferSize);

  PaError err = Pa_OpenStream(&_stream,
                             paNoDevice,//no input
                             0,         
                             paFloat32,  
                             NULL,
                             Pa_GetDefaultOutputDeviceID(),//default output device
                             1,          //mono
                             paFloat32,      
                             NULL,
                             Engine::signalInfo().sampleRate(),
                             framesPerBuffer,
                             nbBuffers,             
                             paClipOff,      /* we won't output out of range samples so don't bother clipping them*/
                             portAudioCallback,
                             this);


  if (err != paNoError)
    std::cout << Pa_GetErrorText(err) << std::endl;
  else
    std::cout << "Opening PortAudio Stream...done" << std::endl;
}

int Gear_AudioOutput::portAudioCallback(void *, void *output_buffer, unsigned long frames_per_buffer,
                                        PaTimestamp, void *user_data)
{
  Gear_AudioOutput *parent = (Gear_AudioOutput*)user_data;
    
  SignalType& lbuffer = parent->_lBuffer;
  int& lindex = parent->_lBufferIndex;
  
  float *out = (float*)output_buffer;

  for (unsigned int i=0; i<frames_per_buffer; i++ )
    *out++ = lbuffer[lindex++];        

  lindex %= parent->_ringBufferSize;

  return 0;
}


