/* Gear_Slider.cpp
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

#include "Gear_Slider.h"
#include "GearMaker.h"
#include "GearGui_Slider.h"
#include "ControlSlider.h"
#include "DroneMath.h"
	
#include "MidiEngine.h"

#include "Engine.h"


#include <qdom.h>
#include <iostream>
#include <sstream>

extern "C" {
Gear* makeGear()
{
  return new Gear_Slider();
}


}

const float  Gear_Slider::DEFAULT_VALUE = 1.0f;

const QString Gear_Slider::SETTING_LOWERBOUND = "Lower Bound";
const QString Gear_Slider::SETTING_HIGHERBOUND = "Higher Bound";

const QString Gear_Slider::SETTING_ACCEPTMIDI = "Accept Midi";
const QString Gear_Slider::SETTING_MIDICHANNEL = "Midi Channel";
const QString Gear_Slider::SETTING_MIDICONTROLLER = "Midi controller";

Gear_Slider::Gear_Slider() : GearControl(schema, "Slider"),_acceptHint(true)
{

  addPlug(_VALUE_OUT = new PlugOut<ValueType>(this, "Value", true));

  _settings.add(Property::FLOAT, SETTING_HIGHERBOUND)->valueFloat(100.0f);
  _settings.add(Property::FLOAT, SETTING_LOWERBOUND)->valueFloat(0.0f);

  _settings.add(Property::BOOL, SETTING_ACCEPTMIDI)->valueBool(false);
  _settings.add(Property::INT, SETTING_MIDICHANNEL)->valueInt(0);
  _settings.add(Property::INT, SETTING_MIDICONTROLLER)->valueInt(0);

  setValue(DEFAULT_VALUE);

}

Gear_Slider::~Gear_Slider()
{

}

void Gear_Slider::onUpdateSettings()
{
  //validation
  float low = _settings.get(Gear_Slider::SETTING_LOWERBOUND)->valueFloat();
  float hi = _settings.get(Gear_Slider::SETTING_HIGHERBOUND)->valueFloat();    
  if (low>hi)
  {
    _settings.get(Gear_Slider::SETTING_LOWERBOUND)->valueFloat(hi);
    _settings.get(Gear_Slider::SETTING_HIGHERBOUND)->valueFloat(low);    
  }

  //range have changed
  //set the value, to force clamping if needed
  setValue(getValue());

  //then we need to redraw the gearGui
  getGearGui()->rebuildLayout();

  //tell the control slider to update his range and value
  //getControl()->onUpdate();

  _acceptHint = false;
}

void Gear_Slider::onPlugConnected(AbstractPlug *plug, AbstractPlug*)
{
  if (plug == _VALUE_OUT && _acceptHint)
  {
    const ValueType *tmpType = static_cast<const ValueType*>(plug->firstConnectedPlug()->abstractHintType());
    _settings.get(Gear_Slider::SETTING_LOWERBOUND)->valueFloat(tmpType->minValue()); 
    _settings.get(Gear_Slider::SETTING_HIGHERBOUND)->valueFloat(tmpType->maxValue());
    setValue(tmpType->value());
    onUpdateSettings();
  }
}

void Gear_Slider::onPlugDisconnected(AbstractPlug* plug, AbstractPlug*)
{
   if (plug == _VALUE_OUT)
     _acceptHint=true;
} 


void Gear_Slider::setValue(float value)
{
  //clamp value to range
  float low = _settings.get(Gear_Slider::SETTING_LOWERBOUND)->valueFloat();
  float hi = _settings.get(Gear_Slider::SETTING_HIGHERBOUND)->valueFloat();    

  value = CLAMP(value, low, hi);

  _VALUE_OUT->type()->setValue(value);
}

void Gear_Slider::runAudio()
{
  bool accept_midi = _settings.get(Gear_Slider::SETTING_ACCEPTMIDI)->valueBool();
  if(accept_midi)
  {
    MidiMessage* msg;
    std::vector<MidiMessage*> messages = MidiEngine::getInstance().getMessages();

    if(messages.size())
    {
      int channel = _settings.get(Gear_Slider::SETTING_MIDICHANNEL)->valueInt();
      int controller = _settings.get(Gear_Slider::SETTING_MIDICONTROLLER)->valueInt();

      // we only consider the LAST controller value
      float low = _settings.get(Gear_Slider::SETTING_LOWERBOUND)->valueFloat();
      float hi = _settings.get(Gear_Slider::SETTING_HIGHERBOUND)->valueFloat();    
      float lastValue = -1,lastStamp;
      for(unsigned int i=0;i<messages.size();i++)
      {
        msg = messages[i];
        if(msg->isControllerChange() && msg->getChannel()==channel  && msg->getController()==controller)
        {
          lastValue = (float)msg->getControllerValue() / 127;
          lastStamp = msg->getStamp();
        }
      }
      
      if(lastValue!=-1)
      {
        //std::cout<<"set Value !! :"<<lastValue<<" "<<lastStamp<<std::endl;
        setValue(low + (hi-low)*lastValue);
        
        // can't call this from here since we're in a different thread than the GUI
        //getGearGui()->reDraw();
      }
    }
  }
}


void Gear_Slider::internalSave(QDomDocument &doc, QDomElement &gearElem)
{
  std::ostringstream strValue;

  strValue << getValue();
  QDomAttr attrSliderPos = doc.createAttribute("SliderPos");
  attrSliderPos.setValue(strValue.str().c_str());
  gearElem.setAttributeNode(attrSliderPos);

}

void Gear_Slider::internalLoad(QDomElement &gearElem)
{
  setValue(gearElem.attribute("SliderPos","").toFloat());        
  _acceptHint=false;
}

