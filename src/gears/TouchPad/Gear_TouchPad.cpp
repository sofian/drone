/* Gear_TouchPad.cpp
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

#include "Gear_TouchPad.h"
#include "GearMaker.h"
#include "GearGui_TouchPad.h"
#include "Math.h"

#include "Engine.h"

#include <qdom.h>
#include <iostream>
#include <sstream>

extern "C" {
Gear* makeGear(Engine *engine, std::string name)
{
  return new Gear_TouchPad(engine,name);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "TouchPad";
  gearInfo.classification = GearClassifications::control().instance();
  return gearInfo;
}
}

const float  Gear_TouchPad::DEFAULT_VALUE = 0.5f;

const std::string Gear_TouchPad::SETTING_HORIZONTAL_LOWERBOUND = "Horizontal lower Bound";
const std::string Gear_TouchPad::SETTING_HORIZONTAL_HIGHERBOUND = "Horizontal higher Bound";
const std::string Gear_TouchPad::SETTING_VERTICAL_LOWERBOUND = "Vertical lower Bound";
const std::string Gear_TouchPad::SETTING_VERTICAL_HIGHERBOUND = "Vertical higher Bound";

Gear_TouchPad::Gear_TouchPad(Engine *engine, std::string name) : Gear(engine, "TouchPad", name),
                                                                 _acceptHorizontalHint(true),
                                                                 _acceptVerticalHint(true)
{
  addPlug(_VALUE_H_OUT = new PlugOut<ValueType>(this, "HValue"));
  addPlug(_VALUE_V_OUT = new PlugOut<ValueType>(this, "VValue"));

  _settings.add(Property::FLOAT, SETTING_HORIZONTAL_HIGHERBOUND)->valueFloat(1.0f);
  _settings.add(Property::FLOAT, SETTING_HORIZONTAL_LOWERBOUND)->valueFloat(0.0f);
  _settings.add(Property::FLOAT, SETTING_VERTICAL_HIGHERBOUND)->valueFloat(1.0f);
  _settings.add(Property::FLOAT, SETTING_VERTICAL_LOWERBOUND)->valueFloat(0.0f);

  setHorizontalValue(DEFAULT_VALUE);
  setVerticalValue(DEFAULT_VALUE);
}

Gear_TouchPad::~Gear_TouchPad()
{

}

bool Gear_TouchPad::ready()
{
  return(_VALUE_H_OUT->connected() || _VALUE_V_OUT->connected());
}

void Gear_TouchPad::onUpdateSettings()
{
  onUpdateHorizontal();
  onUpdateVertical();
}

void Gear_TouchPad::onUpdateHorizontal()
{
  float xlow = _settings.get(Gear_TouchPad::SETTING_HORIZONTAL_LOWERBOUND)->valueFloat();
  float xhi = _settings.get(Gear_TouchPad::SETTING_HORIZONTAL_HIGHERBOUND)->valueFloat();    
  if (xlow>xhi)
  {
    _settings.get(Gear_TouchPad::SETTING_HORIZONTAL_LOWERBOUND)->valueFloat(xhi);
    _settings.get(Gear_TouchPad::SETTING_HORIZONTAL_HIGHERBOUND)->valueFloat(xlow);    
  }
  //set the value, to force clamping if needed
  setHorizontalValue(getHorizontalValue());

  //then we need to redraw the gearGui
  getGearGui()->reDraw();

  _acceptHorizontalHint = false;
}

void Gear_TouchPad::onUpdateVertical()
{
  float ylow = _settings.get(Gear_TouchPad::SETTING_VERTICAL_LOWERBOUND)->valueFloat();
  float yhi = _settings.get(Gear_TouchPad::SETTING_VERTICAL_HIGHERBOUND)->valueFloat();    
  if (ylow>yhi)
  {
    _settings.get(Gear_TouchPad::SETTING_VERTICAL_LOWERBOUND)->valueFloat(yhi);
    _settings.get(Gear_TouchPad::SETTING_VERTICAL_HIGHERBOUND)->valueFloat(ylow);    
  }

  //set the value, to force clamping if needed
  setVerticalValue(getVerticalValue());

  //then we need to redraw the gearGui
  getGearGui()->reDraw();

  _acceptVerticalHint = false;
}

void Gear_TouchPad::onPlugConnected(AbstractPlug *plug)
{
  if (plug == _VALUE_H_OUT && _acceptHorizontalHint)
  {
    const ValueType *tmpType = static_cast<const ValueType*>(plug->firstConnectedPlug()->abstractHintType());
    _settings.get(Gear_TouchPad::SETTING_HORIZONTAL_LOWERBOUND)->valueFloat(tmpType->minValue()); 
    _settings.get(Gear_TouchPad::SETTING_HORIZONTAL_HIGHERBOUND)->valueFloat(tmpType->maxValue());
    setHorizontalValue(tmpType->value());
    onUpdateHorizontal();
  }

  if (plug == _VALUE_V_OUT && _acceptVerticalHint)
  {
    const ValueType *tmpType = static_cast<const ValueType*>(plug->firstConnectedPlug()->abstractHintType());
    _settings.get(Gear_TouchPad::SETTING_VERTICAL_LOWERBOUND)->valueFloat(tmpType->minValue()); 
    _settings.get(Gear_TouchPad::SETTING_VERTICAL_HIGHERBOUND)->valueFloat(tmpType->maxValue());
    setVerticalValue(tmpType->value());
    onUpdateVertical();
  }

  
}

void Gear_TouchPad::onPlugDisconnected(AbstractPlug* plug)
{
  if (plug == _VALUE_H_OUT)
    _acceptHorizontalHint=true;
  if (plug == _VALUE_V_OUT)
    _acceptVerticalHint=true;
} 


void Gear_TouchPad::setHorizontalValue(float value)
{
  //clamp value to range
  float low = _settings.get(Gear_TouchPad::SETTING_HORIZONTAL_LOWERBOUND)->valueFloat();
  float hi = _settings.get(Gear_TouchPad::SETTING_HORIZONTAL_HIGHERBOUND)->valueFloat();    

  value = CLAMP(value, low, hi);

  _VALUE_H_OUT->type()->setValue(value);
}

void Gear_TouchPad::setVerticalValue(float value)
{
  //clamp value to range
  float low = _settings.get(Gear_TouchPad::SETTING_VERTICAL_LOWERBOUND)->valueFloat();
  float hi = _settings.get(Gear_TouchPad::SETTING_VERTICAL_HIGHERBOUND)->valueFloat();    

  value = CLAMP(value, low, hi);

  _VALUE_V_OUT->type()->setValue(value);
}

void Gear_TouchPad::runAudio()
{
}

GearGui *Gear_TouchPad::createGearGui(QCanvas *canvas)
{                
  return new GearGui_TouchPad(this, canvas);
}

void Gear_TouchPad::save(QDomDocument &doc, QDomElement &gearElem)
{
  std::ostringstream strValue;

  strValue << getHorizontalValue();
  QDomAttr attrTouchPadHorizontalPos = doc.createAttribute("TouchPadHorizontalPos");
  attrTouchPadHorizontalPos.setValue(strValue.str().c_str());
  gearElem.setAttributeNode(attrTouchPadHorizontalPos);

  strValue << getVerticalValue();
  QDomAttr attrTouchPadVerticalPos = doc.createAttribute("TouchPadVerticalPos");
  attrTouchPadVerticalPos.setValue(strValue.str().c_str());
  gearElem.setAttributeNode(attrTouchPadVerticalPos);
}

void Gear_TouchPad::load(QDomElement &gearElem)
{
  setHorizontalValue(gearElem.attribute("TouchPadHorizontalPos","").toFloat());
  setVerticalValue(gearElem.attribute("TouchPadVerticalPos","").toFloat());
  _acceptHorizontalHint=false;
  _acceptVerticalHint=false;
}


