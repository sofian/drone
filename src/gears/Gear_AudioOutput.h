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
#include <portaudio.h>



class PlugSignal;

class Gear_AudioOutput : public Gear  
{
public:

//CONFIG PARAMETERS
  static const std::string SETTING_FRAMES_PER_BUFFER;
  static const std::string SETTING_NB_BUFFERS;
//

  Gear_AudioOutput(Schema *schema, std::string uniqueName);
  virtual ~Gear_AudioOutput();

  void prePlay();    
  void postPlay();    

  void init();
  void runAudio();    

  PlugIn<SignalType>* AUDIO_IN_LEFT(){return _AUDIO_IN_LEFT;};
  PlugIn<SignalType>* AUDIO_IN_RIGHT(){return _AUDIO_IN_RIGHT;};

  bool ready();

protected:
  void onUpdateSettings();

private:

//PLUGS
  PlugIn<SignalType> *_AUDIO_IN_LEFT;
  PlugIn<SignalType> *_AUDIO_IN_RIGHT;
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

  static int portAudioCallback(void *, void *output_buffer, unsigned long frames_per_buffer,
                               PaTimestamp, void *user_data);

};

#endif

