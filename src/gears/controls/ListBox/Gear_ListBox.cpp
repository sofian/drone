/* Gear_ListBox.cpp
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#include "Gear_ListBox.h"

#include "DroneMath.h"

#include "Engine.h"

#include <qdom.h>
#include <iostream>
#include <sstream>

extern "C" {
Gear* makeGear()
{
  return new Gear_ListBox();
}


}

const QString Gear_ListBox::SETTING_NELEMS = "Number of elements";
const QString Gear_ListBox::SETTING_LABELS = "Labels of elements";

Gear_ListBox::Gear_ListBox() : 
  Gear("ListBox"),_acceptHint(true)
{
  addPlug(_VALUE_OUT = new PlugOut<EnumType>(this, "Value", true));

  _settings.add(Property::FLOAT, SETTING_NELEMS)->valueInt(1);
   // XXX temporary hack to save labels
  _settings.add(Property::STRING, SETTING_LABELS)->valueStr("");
  _labels.resize(1);
  _labels[0] = "";

  setValue(0);
}

Gear_ListBox::~Gear_ListBox()
{

}

void Gear_ListBox::onUpdateSettings()
{
  _VALUE_OUT->type()->resize(_settings.get(Gear_ListBox::SETTING_NELEMS)->valueInt());

  //set the value, to force clamping if needed
  setValue(getValue());

  //then we need to redraw the gearGui
//  getGearGui()->rebuildLayout();

  // XXX temporary hack to save labels, to be removed ultimately
  char str[1000];
  strcpy(str, _settings.get(Gear_ListBox::SETTING_LABELS)->valueStr());
  char *tok = strtok(str, ",");
  _labels.resize(0);
  while (tok != NULL) // parse comma-separated array
  {
    _labels.push_back(tok);
    tok = strtok (NULL, ",");
  }
  
  std::cout << "labels : " << _settings.get(Gear_ListBox::SETTING_LABELS)->valueStr().latin1() << std::endl;
  std::cout << "value out type: " << _VALUE_OUT->type()->size() <<std::endl;
  std::cout << "label: " << _labels.size() <<std::endl;

  ASSERT_ERROR(_labels.size() == _VALUE_OUT->type()->size());

  _acceptHint = false;
}

void Gear_ListBox::onPlugConnected(AbstractPlug *plug, AbstractPlug *)
{
  if (plug == _VALUE_OUT && _acceptHint)
  {
    const EnumType *tmpType = static_cast<const EnumType*>(plug->firstConnectedPlug()->abstractHintType());
    _settings.get(Gear_ListBox::SETTING_NELEMS)->valueInt((int)tmpType->size());
    _VALUE_OUT->type()->resize(tmpType->size());
    _labels.resize(tmpType->size());
    QString labels = "";
    if (tmpType->size() > 0)
    {
      QString label = tmpType->label(0);
      labels += label;
      _VALUE_OUT->type()->setLabel(0, label);
      for (int i=1; i<(int)tmpType->size(); ++i)
      {
        label = tmpType->label(i);
        labels += "," + label;
        _VALUE_OUT->type()->setLabel(i, label);
      }
    }
    
    _settings.get(Gear_ListBox::SETTING_LABELS)->valueStr(labels);
    setValue(tmpType->value());
    onUpdateSettings();
  }
}

void Gear_ListBox::onPlugDisconnected(AbstractPlug* plug, AbstractPlug *)
{
   if (plug == _VALUE_OUT)
     _acceptHint=true;
} 

void Gear_ListBox::setValue(int value)
{
  //clamp value to range
  int size = _settings.get(Gear_ListBox::SETTING_NELEMS)->valueInt();

  value = CLAMP(value, 0, size-1);

  _VALUE_OUT->type()->setValue(value);
}

void Gear_ListBox::runAudio()
{

}

void Gear_ListBox::internalSave(QDomDocument &doc, QDomElement &gearElem)
{
  std::ostringstream strValue;

  strValue << getValue();
  QDomAttr attrListBoxPos = doc.createAttribute("ListBoxPos");
  attrListBoxPos.setValue(strValue.str().c_str());
  gearElem.setAttributeNode(attrListBoxPos);

}

void Gear_ListBox::internalLoad(QDomElement &gearElem)
{
  setValue(gearElem.attribute("ListBoxPos","").toInt());        
  _acceptHint=false;
}

