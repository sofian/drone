/* Gear_AudioInput.h
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

#ifndef GEAR_AUDIOINPUT_INCLUDED
#define GEAR_AUDIOINPUT_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include <portaudio.h>

class PlugSignal;

class Gear_AudioInput : public Gear  
{
public:

//CONFIG PARAMETERS
  static const std::string SETTING_FRAMES_PER_BUFFER;
  static const std::string SETTING_NB_BUFFERS;
//

  Gear_AudioInput(Schema *schema, std::string uniqueName);
  virtual ~Gear_AudioInput();

  void prePlay();    
  void postPlay();    
  
  void init();
  void runAudio();

  bool ready();

protected:
  void onUpdateSettings();


private:

//PLUGS
  PlugOut<SignalType>* _AUDIO_OUT_LEFT;
  PlugOut<SignalType>* _AUDIO_OUT_RIGHT;
//


//portaudio

  static const int DEFAULT_FRAMES_PER_BUFFER;
  static const int DEFAULT_NB_BUFFERS;

  PortAudioStream *_stream;

  void initPortAudio();    

  int _ringBufferSize;

  SignalType _lBuffer;
  SignalType _rBuffer;

  int _lBufferIndex;

  int _readIndex;

  static int portAudioCallback(void *input_buffer, void *, unsigned long frames_per_buffer,
                               PaTimestamp, void *user_data);

  pthread_mutex_t *_mutex;
};

#endif

