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


void Property::valueStr(std::string value)
{
  _value.clear();  
  _value.push_back(value);
}

void Property::valueStrList(const std::vector<std::string> &value)
{
  _value = value;
  if(!_value.size())
    _value.push_back("");
};

void Property::valueFloat(float value)
{
  std::ostringstream str;
  str << value;
  _value.clear();
  _value.push_back(str.str());
}

void Property::valueBool(bool value)
{
  std::ostringstream str;
  str << std::noboolalpha << value;
  _value.clear();
  _value.push_back(str.str());
}

void Property::valueInt(int value)
{
  std::ostringstream str;
  str << value;
  _value.clear();
  _value.push_back(str.str());
}

int Property::valueInt()
{
  return atoi(_value[0].c_str());
}

float Property::valueFloat()
{    
  return atof(_value[0].c_str());
}

bool Property::valueBool()
{
  return _value[0] == "1" ? true : false;
}

Property* Properties::add(Property::eType type, std::string name)
{
  //no duplicate
  ASSERT_ERROR(_properties.find(name) == _properties.end());

  Property *newProperty = new Property(type, name);

  _properties[name] = newProperty;

  return newProperty;
}


Property* Properties::get(const std::string& name)
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
  QDomElement propertiesElem = doc.createElement(XML_TAGNAME.c_str());
  parent.appendChild(propertiesElem);

  QDomAttr propertieAttr;
  for (std::map<std::string, Property*>::iterator it = _properties.begin(); it != _properties.end(); ++it)
  {    
    // to save string lists, we save each value the format property_name[index]
    const std::vector<std::string> &lst = it->second->valueStrList();
    if(lst.size()>1)
    {
      
      for(unsigned int i=0;i<lst.size(); i++)
      {
        QString index;
        index.sprintf("%i",i);
        propertieAttr = doc.createAttribute(QString(it->second->name().c_str()).replace(' ', WHITESPACE_REPLACEMENT)+index);
        propertieAttr.setValue(lst[i].c_str());
        propertiesElem.setAttributeNode(propertieAttr);
      }
    }
    else
    {
      propertieAttr = doc.createAttribute(QString(it->second->name().c_str()).replace(' ', WHITESPACE_REPLACEMENT));
      propertieAttr.setValue(it->second->valueStr().c_str());
      propertiesElem.setAttributeNode(propertieAttr);
    }
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
    // we need to replace whitespaces with another char for xml attributes
    QString name = QString(it->second->name().c_str()).replace(' ', WHITESPACE_REPLACEMENT);
    std::ostringstream oss;
    // to save string lists, we save each value the format name[index]
    // we start by checking if there is a property named prop0, in which case we expect a string list
    bool exists=true;
    int index=0;
    std::vector<std::string> strlist;
    for(;exists; index++)
    {
      std::ostringstream oss;
      oss<<name.toStdString()<<index;      
      QString value = propertiesElem.attribute(oss.str().c_str());
      
      if(value==QString::null)
        exists=false;      
      else
        strlist.push_back(value.ascii());
    }

    if(index==1)//if it was not a stringlist, simply get the propertie value
      it->second->valueStr(propertiesElem.attribute(QString(it->second->name().c_str()).replace(' ', WHITESPACE_REPLACEMENT),"").ascii());
    else
      it->second->valueStrList(strlist);
  }    


}
