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
const QString Properties::XML_TAGNAME = "Properties";


void Property::valueStr(QString value)
{
  _value.clear();  
  _value.push_back(value);
}

void Property::valueStrList(const QList<QString> &value)
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
  _value.push_back(str.str().c_str());
}

void Property::valueBool(bool value)
{
  std::ostringstream str;
  str << std::noboolalpha << value;
  _value.clear();
  _value.push_back(str.str().c_str());
}

void Property::valueInt(int value)
{
  std::ostringstream str;
  str << value;
  _value.clear();
  _value.push_back(str.str().c_str());
}

int Property::valueInt()
{
  return atoi(_value[0]);
}

float Property::valueFloat()
{    
  return atof(_value[0]);
}

bool Property::valueBool()
{
  return _value[0] == "1" ? true : false;
}

Property* Properties::add(Property::eType type, QString name)
{
  //no duplicate
  ASSERT_ERROR(_properties.find(name) == _properties.end());

  Property *newProperty = new Property(type, name);

  _properties[name] = newProperty;

  return newProperty;
}


Property* Properties::get(QString name)
{
  if (_properties.find(name) == _properties.end())
    return NULL;

  return _properties[name];
}


void Properties::getAll(QList<Property*> *properties)
{
 QMap<QString, Property*>::const_iterator i = _properties.constBegin();
 while (i != _properties.constEnd()) {
    properties->push_back(i.value());
  }
}

void Properties::save(QDomDocument &doc, QDomElement &parent)
{               
  QDomElement propertiesElem = doc.createElement(XML_TAGNAME);
  parent.appendChild(propertiesElem);

  QDomAttr propertieAttr;
  for (QMap<QString, Property*>::iterator it = _properties.begin(); it != _properties.end(); ++it)
  {    
    // to save string lists, we save each value the format property_name[index]
    const QList<QString> &lst = it.value()->valueStrList();
    if(lst.size()>1)
    {
      
      for(int i=0;i<lst.size(); i++)
      {
        QString index;
        index.sprintf("%i",i);
        propertieAttr = doc.createAttribute(QString(it.value()->name()).replace(' ', WHITESPACE_REPLACEMENT)+index);
        propertieAttr.setValue(lst[i]);
        propertiesElem.setAttributeNode(propertieAttr);
      }
    }
    else
    {
      propertieAttr = doc.createAttribute(QString(it.value()->name()).replace(' ', WHITESPACE_REPLACEMENT));
      propertieAttr.setValue(it.value()->valueStr());
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

  for (QMap<QString, Property*>::iterator it = _properties.begin(); it != _properties.end(); ++it)
  {
    // we need to replace whitespaces with another char for xml attributes
    QString name = QString(it.value()->name()).replace(' ', WHITESPACE_REPLACEMENT);
    std::ostringstream oss;
    // to save string lists, we save each value the format name[index]
    // we start by checking if there is a property named prop0, in which case we expect a string list
    bool exists=true;
    int index=0;
    QList<QString> strlist;
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
      it.value()->valueStr(propertiesElem.attribute(QString(it.value()->name()).replace(' ', WHITESPACE_REPLACEMENT),"").ascii());
    else
      it.value()->valueStrList(strlist);
  }    


}
