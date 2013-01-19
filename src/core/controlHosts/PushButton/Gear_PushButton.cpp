/* Gear_PushButton.cpp
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

#include "Gear_PushButton.h"
#include "GearMaker.h"
#include "DroneMath.h"
#include "MidiEngine.h"
#include "Engine.h"

#include <qdom.h>
#include <iostream>
#include <sstream>

Gear* makeGear_PushButton()
{
  return new Gear_PushButton();
}
 
const QString Gear_PushButton::SETTING_OFFVALUE = "Off Value";
const QString Gear_PushButton::SETTING_ONVALUE = "On Value";
const QString Gear_PushButton::SETTING_ONESHOT = "One Shot";

const QString Gear_PushButton::SETTING_ACCEPTMIDI = "Accept Midi";
const QString Gear_PushButton::SETTING_MIDICHANNEL = "Midi Channel";
const QString Gear_PushButton::SETTING_MIDINOTE = "Midi Note";

Gear_PushButton::Gear_PushButton() : 
  GearControl("PushButton"),_acceptHint(true)
{

  addPlug(_VALUE_OUT = new PlugOut<ValueType>(this, "Value", true));

  _settings.add(Property::FLOAT, SETTING_ONVALUE)->valueFloat(1.0f);
  _settings.add(Property::FLOAT, SETTING_OFFVALUE)->valueFloat(0.0f);
  _settings.add(Property::FLOAT, SETTING_ONESHOT)->valueFloat(0.0f);

  _settings.add(Property::BOOL, SETTING_ACCEPTMIDI)->valueBool(false);
  _settings.add(Property::INT, SETTING_MIDICHANNEL)->valueInt(0);
  _settings.add(Property::INT, SETTING_MIDINOTE)->valueInt(0);



  setState(OFF);
  _lastWasOn=false;
  _havePendingRelease=_haveUnconsumedClick=false;
}

Gear_PushButton::~Gear_PushButton()
{

}

void Gear_PushButton::onPlugConnected(AbstractPlug *plug, AbstractPlug*)
{
  if (plug == _VALUE_OUT && _acceptHint)
  {
  }
}

void Gear_PushButton::onPlugDisconnected(AbstractPlug* plug, AbstractPlug*)
{
   if (plug == _VALUE_OUT)
     _acceptHint=true;
} 


void Gear_PushButton::setState(eButtonState state)
{
  _state=state;
  if(state==OFF)
    if(!_haveUnconsumedClick)
    {
      _VALUE_OUT->type()->setValue(_settings.get(Gear_PushButton::SETTING_OFFVALUE)->valueFloat());
    }
    else
      _havePendingRelease=true;

  else
  { 
    _VALUE_OUT->type()->setValue(_settings.get(Gear_PushButton::SETTING_ONVALUE)->valueFloat());
    _haveUnconsumedClick=true;
    _havePendingRelease=false;
  }
}

void Gear_PushButton::runVideo()
{

 bool accept_midi = _settings.get(Gear_PushButton::SETTING_ACCEPTMIDI)->valueBool();
 if(accept_midi)
 {
   bool noteOff;
   MidiMessage* msg;
   std::vector<MidiMessage*> messages = MidiEngine::getInstance().getMessages();

   if(messages.size())
   {
     int channel = _settings.get(Gear_PushButton::SETTING_MIDICHANNEL)->valueInt();
     int Note = _settings.get(Gear_PushButton::SETTING_MIDINOTE)->valueInt();

     // we only consider the LAST Note value
     float on =_settings.get(Gear_PushButton::SETTING_ONVALUE)->valueFloat();
     float off =_settings.get(Gear_PushButton::SETTING_OFFVALUE)->valueFloat();
     
     float highValue = -1,lastStamp;
     noteOff=false;
     for(int i=0;i<messages.size();i++)
     {
       msg = messages[i];
       if(msg->isNoteEvent() && msg->getChannel()==channel  && msg->getNote()==Note)
       {
         std::cerr<<"have event!!!!!!!"<<std::endl;
         if(msg->getNoteValue()==0)
           noteOff=true;
         else
         {
           noteOff=false;
           highValue = (float)msg->getNoteValue() / 127;
         }
         lastStamp = msg->getStamp();
       }
     }

     if(highValue!=-1)
     {
       //std::cout<<"set Value !! :"<<lastValue<<" "<<lastStamp<<std::endl;
       setState(ON);

       // can't call this from here since we're in a different thread than the GUI
       //getGearGui()->reDraw();
     }
   }
   if (noteOff)
   {
     _havePendingRelease=true;
   }
 }




// keyboard part




  if(_settings.get(Gear_PushButton::SETTING_ONESHOT)->valueBool() && _lastWasOn)
    setState(OFF);
  if(_VALUE_OUT->type()->value()==_settings.get(Gear_PushButton::SETTING_ONVALUE)->valueFloat())
    _lastWasOn=true;
  else
    _lastWasOn=false;  
  _haveUnconsumedClick=false;
  if(_havePendingRelease)
    setState(OFF);
}

