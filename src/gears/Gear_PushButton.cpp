#include "Gear_PushButton.h"
#include "GearMaker.h"
#include "GearGui_PushButton.h"
#include "Math.h"

#include "Engine.h"

#include <qdom.h>
#include <iostream>
#include <sstream>

Register_Gear(MAKERGear_PushButton, Gear_PushButton, "PushButton")

const std::string Gear_PushButton::SETTING_OFFVALUE = "Off Value";
const std::string Gear_PushButton::SETTING_ONVALUE = "On Value";
const std::string Gear_PushButton::SETTING_ONESHOT = "One Shot";

Gear_PushButton::Gear_PushButton(Engine *engine, std::string name) : Gear(engine, "PushButton", name),_acceptHint(true)
{

  addPlug(_VALUE_OUT = new PlugOut<ValueType>(this, "Value"));

  _settings.add(Property::FLOAT, SETTING_ONVALUE)->valueFloat(1.0f);
  _settings.add(Property::FLOAT, SETTING_OFFVALUE)->valueFloat(0.0f);
  _settings.add(Property::FLOAT, SETTING_ONESHOT)->valueFloat(0.0f);

  setState(OFF);
  _lastWasOn=false;
  _havePendingRelease=_haveUnconsumedClick=false;
}

Gear_PushButton::~Gear_PushButton()
{

}

bool Gear_PushButton::ready()
{
  return(_VALUE_OUT->connected());
}

void Gear_PushButton::onUpdateSettings()
{
  //then we need to redraw the gearGui
  getGearGui()->reDraw();
}

void Gear_PushButton::onPlugConnected(AbstractPlug *plug)
{
  if (plug == _VALUE_OUT && _acceptHint)
  {
  }
}

void Gear_PushButton::onPlugDisconnected(AbstractPlug* plug)
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

GearGui *Gear_PushButton::createGearGui(QCanvas *canvas)
{                
  return new GearGui_PushButton(this, canvas);
}
