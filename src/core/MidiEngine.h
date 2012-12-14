/* MidiEngine.h
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

#ifndef MIDIENGINE_INCLUDED
#define MIDIENGINE_INCLUDED

#include <vector>
#include <string>
#include <pthread.h>

#include "RtMidi/RtMidi.h"

// #include "SignalInfo.h"
// #include "VideoInfo.h"
// #include "DroneMath.h"
// #include "error.h"
// #include "Schema.h"

// class Gear;
// class AbstractPlug;
// class QDomDocument;
// class QDomElement;

class MidiMessage
{

public :
  MidiMessage(std::vector<unsigned char> vec, float stamp);

  enum  {
    NOTE_OFF  =0x80,
    NOTE_ON   =0x90,
    POLY_PRESSURE =0xa0,
    CONTROL_CHANGE  =0xb0,
    PROGRAM_CHANGE  =0xc0,
    CHANNEL_PRESSURE=0xd0,
    PITCH_BEND  =0xe0,
    SYSEX_START =0xf0,
    MTC   =0xf1,
    SONG_POSITION =0xf2,
    SONG_SELECT =0xf3,
    TUNE_REQUEST  =0xf6,
    SYSEX_END =0xf7,
    RESET   =0xff,  // 0xff never used as reset in a MIDIMessage
    META_EVENT  =0xff // 0xff is for non MIDI messages
  };

  int getChannel(){return (_status & 0x0F);}
  bool isChannelMsg(){  return (_status >= 0x80 && _status <= 0xE0); }
  
  unsigned char isControllerChange(){return ((_status & 0xf0)==CONTROL_CHANGE);}
  unsigned char getController(){return _byte1;}
  unsigned char getControllerValue(){return _byte2;}
  
  unsigned char isNoteEvent(){return (_status == 144);}
  unsigned char getNote(){return _byte1;}
  unsigned char getNoteValue(){return _byte2;}


  float getStamp(){return _stamp;}

protected:
  float _stamp;
  unsigned char _status;
  unsigned char _byte1;
  unsigned char _byte2;
  unsigned char _byte3;
};


class MidiEngine
{
private:

  MidiEngine();

public:

  virtual ~MidiEngine();

  void purgeAndGetNew();  

  static MidiEngine& getInstance();

  std::vector<MidiMessage*> getMessages();

  void clearAllMessages();

protected:

  std::vector<MidiMessage*> _messages;
  RtMidiIn *_midiin;


};

#endif
