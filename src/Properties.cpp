#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>

#include <qdom.h>

#include "Properties.h"

const char Properties::WHITESPACE_REPLACEMENT = '_';

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
    assert(_properties.find(name) == _properties.end());
        
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
    QDomElement propertiesElem = doc.createElement("Properties");
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
    QDomNodeList propertiesNodes = parentElem.elementsByTagName("Properties");
    
    if (propertiesNodes.count()==1)
    {
        QDomElement propertiesElem = propertiesNodes.item(0).toElement();

        for (std::map<std::string, Property*>::iterator it = _properties.begin(); it != _properties.end(); ++it)    
        {                
            //we need to replace whitespaces with another char for xml attributes
            it->second->valueStr(propertiesElem.attribute(QString(it->second->name().c_str()).replace(' ', WHITESPACE_REPLACEMENT),"").ascii());
        }    
                
    }
    else
        std::cout << "Bad DroneSchema : problem with properties tag" << std::endl;

}
