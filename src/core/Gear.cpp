/* Gear.cpp
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

#include "Gear.h"

#include "GearGui.h"
#include "Schema.h"

#include <qdom.h>
#include <iostream>
#include <sstream>

Gear::Gear(Schema *schema, std::string type, std::string uniqueName) : 
_parentSchema(schema), 
_Type(type), 
_Name(type),//default name to typeName
_uniqueName(uniqueName),
_gearGui(NULL)
{
  ASSERT_ERROR_MESSAGE(_parentSchema!=0, "NULL schema passed to gear");
}

Gear::~Gear()
{    
  for (std::list<AbstractPlug*>::iterator it=_plugs.begin(); it != _plugs.end(); ++it)
    delete (*it);
}

void Gear::unSynch()
{
  _parentSchema->unSynch();
}

void Gear::internalPrePlay()
{
  prePlay();
}

void Gear::internalPostPlay()
{
  postPlay();
}

void Gear::internalInit()
{
  std::cout << "__________________________________________" << std::endl;
  std::cout << _Type << std::endl;
  std::cout << "������������������������������������������" << std::endl;

  for (std::list<AbstractPlug*>::iterator it=_plugs.begin(); it != _plugs.end(); ++it)
    (*it)->init();

  _gearGui = createGearGui(NULL);

  //call the virtual method
  init();
}


/**
 * Calls the destructor for the plug and remove it 
 * from the gear plugs
 * 
 * @param plug   the plug to delete
 */
void Gear::deletePlug(AbstractPlug *plug)
{
  delete plug;
  _plugs.remove(plug);
}


/**
 * Adds the specified AbstractPlug to the Gear. 
 * This method will fail if a plug with the same name already
 * exist for this gear.
 * 
 * @param plug   the plug to add
 * 
 * @return Success : pointer on the specified plug
 *         Error : null
 * @see AbstractPlug
 */
AbstractPlug* Gear::addPlug(AbstractPlug* plug)
{    
  //check for duplicate plug name
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
  {
    if ( ((*it)->name() == plug->name()) )
      return NULL;
  }    

  //if plug is input, add only plug for main type
  if (plug->inOut()==IN)
    _plugs.push_back(plug);
  else
    //add main plug and plugs for its subType
    addPlugAndSubPlugs(plug,0);  

  return plug;    
}

void Gear::addPlugAndSubPlugs(AbstractPlug* plug, int level)
{
  std::string str;

  for (int l=0;l<=level;l++)
    str+="..";

  _plugs.push_back(plug);
  int size = plug->abstractType()->nSubTypes();

  for (int i=0;i<size;i++)
  {
    const AbstractType* subtype = (plug->abstractType()->getSubType(i));    
    addPlugAndSubPlugs(new AbstractPlug(this, plug->inOut(), str + subtype->name(), subtype ),level+1);
  }
}

void Gear::getInputs(std::list<AbstractPlug*> &inputs) const
{
  inputs.clear();
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
  {
    if ( ((*it)->inOut() == IN) )
      inputs.push_back(*it);
  }    
}

void Gear::getOutputs(std::list<AbstractPlug*> &outputs) const
{
  outputs.clear();
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
  {
    if ( ((*it)->inOut() == OUT) )
      outputs.push_back(*it);
  }
}

void Gear::getDependencies(std::vector<Gear*> &dependencies) const
{
  std::list<AbstractPlug*> inputs;
  getInputs(inputs);
  std::list<AbstractPlug*> lplug;
  for (std::list<AbstractPlug*>::const_iterator it = inputs.begin();it!=inputs.end();++it)
  {
    (*it)->connectedPlugs(lplug);
    if (lplug.size()!=0 && lplug.back()->parent()->ready())
      dependencies.push_back(lplug.back()->parent());
  }
}

GearGui* Gear::createGearGui(QCanvas *canvas)
{
  return new GearGui(this, canvas);    
}


void Gear::internalSave(QDomDocument &doc, QDomElement &parent)
{               
  QDomElement gearElem = doc.createElement("Gear");
  parent.appendChild(gearElem);

  QDomAttr gearType = doc.createAttribute("Type");
  gearType.setValue(_Type.c_str());
  gearElem.setAttributeNode(gearType);

  QDomAttr gearName = doc.createAttribute("Name");
  gearName.setValue(_Name.c_str());
  gearElem.setAttributeNode(gearName);

  _gearGui->save(doc, gearElem);
  _settings.save(doc, gearElem);

  save(doc, gearElem);

}

void Gear::internalLoad(QDomElement &gearElem)               
{            
  _Name = gearElem.attribute("Name","").ascii();

  _settings.load(gearElem);
  updateSettings();

  load(gearElem);

  _gearGui->load(gearElem);

}

AbstractPlug* Gear::getInput(std::string name) const
{
  std::list<AbstractPlug*> inputs;
  getInputs(inputs);
  for (std::list<AbstractPlug*>::const_iterator it = inputs.begin();it!=inputs.end();++it)
  {
    if ((*it)->name() == name)
      return(*it);
  }

  return NULL;
}

AbstractPlug* Gear::getOutput(std::string name) const
{
  std::list<AbstractPlug*> outputs;
  getOutputs(outputs);
  for (std::list<AbstractPlug*>::const_iterator it = outputs.begin();it!=outputs.end();++it)
  {
    if ((*it)->name() == name)
      return(*it);
  }

  return NULL;
}

GearGui* Gear::getGearGui()
{
  return _gearGui;
}

void Gear::updateSettings()
{
  //call the virtual method that can be overriden
  onUpdateSettings();
}
