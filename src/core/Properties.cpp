/* Properties.cpp
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
#include <sstream>
#include <string>
#include <assert.h>

#include <qdom.h>
#include "XMLHelper.h"

#include "Properties.h"

const char Properties::WHITESPACE_REPLACEMENT = '_';
const std::string Properties::XML_TAGNAME = "Properties";

void Property::valueFloat(float value)
{
  std::ostringstream str;
  str << value;
  _value = str.str();
}

void Property::valueBool(bool value)
{
  std::ostringstream str;
  str << std::noboolalpha << value;
  _value = str.str();
}

void Property::valueInt(int value)
{
  std::ostringstream str;
  str << value;
  _value = str.str();
}

int Property::valueInt()
{
  return atoi(_value.c_str());
}

float Property::valueFloat()
{
  return atof(_value.c_str());
}

bool Property::valueBool()
{
  return _value == "1" ? true : false;
}


Property* Properties::add(Property::eType type, std::string name)
{
  //no duplicate
  ASSERT_ERROR(_properties.find(name) == _properties.end());

  Property *newProperty = new Property(type, name);

  _properties[name] = newProperty;

  return newProperty;
}


Property* Properties::get(std::string name)
{
  if (_properties.find(name) == _properties.end())
    return NULL;

  return _properties[name];
}


void Properties::getAll(std::vector<Property*> *properties)
{
  for (std::map<std::string, Property*>::iterator it = _properties.begin(); it != _properties.end(); ++it)
  {
    properties->push_back(it->second);
  }
}

void Properties::save(QDomDocument &doc, QDomElement &parent)
{               
  QDomElement propertiesElem = doc.createElement(XML_TAGNAME);
  parent.appendChild(propertiesElem);

  QDomAttr propertieAttr;
  for (std::map<std::string, Property*>::iterator it = _properties.begin(); it != _properties.end(); ++it)
  {
    std::cout << it->second->name() << std::endl;
    //we need to replace whitespaces with another char for xml attributes
    propertieAttr = doc.createAttribute(QString(it->second->name().c_str()).replace(' ', WHITESPACE_REPLACEMENT));
    propertieAttr.setValue(it->second->valueStr().c_str());
    propertiesElem.setAttributeNode(propertieAttr);
  }    
}

void Properties::load(QDomElement &parentElem)
{
  //properties
  QDomNode propertiesNodes = XMLHelper::findChildNode(parentElem, XML_TAGNAME);

  if (propertiesNodes.isNull())
  {
    std::cout << "Bad DroneSchema : <Properties> tag not found!" << std::endl;
    return;
  }
  
  QDomElement propertiesElem = propertiesNodes.toElement();

  for (std::map<std::string, Property*>::iterator it = _properties.begin(); it != _properties.end(); ++it)
  {
    //we need to replace whitespaces with another char for xml attributes
    it->second->valueStr(propertiesElem.attribute(QString(it->second->name().c_str()).replace(' ', WHITESPACE_REPLACEMENT),"").ascii());
  }    


}
