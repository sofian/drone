#include <iostream>
#include <sstream>
#include <string>

#include <qdom.h>

#include "Properties.h"

const char Properties::WHITESPACE_REPLACEMENT = '_';



void Properties::add(Property::eType type, std::string name, std::string default_value)
{
    if (_properties.find(name) != _properties.end())
        return;//already exist

    Property *newProperty = new Property(type, name);
    newProperty->value(default_value);

    _properties[name] = newProperty;
}

void Properties::add(Property::eType type, std::string name, int default_value)
{
    std::ostringstream str;
    str << default_value;

    add(type, name, str.str());
}

void Properties::add(Property::eType type, std::string name, float default_value)
{
    std::ostringstream str;
    str << default_value;

    add(type, name, str.str());
}


void Properties::set(std::string name, std::string value)
{
    if (_properties.find(name) == _properties.end())
        return;//not existing
    
    _properties[name]->value(value);
}

void Properties::set(std::string name, int value)
{
    std::ostringstream str;
    str << value;

    set(name,str.str());
}

void Properties::set(std::string name, float value)
{
    std::ostringstream str;
    str << value;

    set(name,str.str());
}


std::string Properties::get(std::string name)
{
    if (_properties.find(name) == _properties.end())
        return "";

    return _properties[name]->value();
}

int Properties::getInt(std::string name)
{
    if (_properties.find(name) == _properties.end())
        return 0;

    return atoi(_properties[name]->value().c_str());
}

float Properties::getFloat(std::string name)
{
    if (_properties.find(name) == _properties.end())
        return 0.0f;

    return atof(_properties[name]->value().c_str());
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
        propertieAttr.setValue(it->second->value().c_str());
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
            it->second->value(propertiesElem.attribute(QString(it->second->name().c_str()).replace(' ', WHITESPACE_REPLACEMENT),"").ascii());
        }    
                
    }
    else
        std::cout << "Bad DroneSchema : problem with properties tag" << std::endl;

}
