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

#include "Schema.h"
#include "GearInfo.h"
#include <QtXml>

#include "XMLHelper.h"
#include <iostream>
#include <sstream>
#include <algorithm>

const QString Gear::XML_TAGNAME = "Gear";

Gear::Gear(Schema *parentSchema, QString type, QString name) : 
_parentSchema(parentSchema), 
_Type(type), 
_name(name),
_ready(false)
{
}

Gear::~Gear()
{
  for (std::list<AbstractPlug*>::iterator it=_plugs.begin(); it != _plugs.end(); ++it)
    delete (*it);
}
/*
void Gear::saveDefinition(QDomDocument& doc)
{
std::cerr<<"void Gear::saveDefinition(QDomDocument& doc)!!!!!"<<std::endl;
  QDomElement gearElem = doc.createElement("GearInfo");
  doc.appendChild(gearElem);
  _gearInfo->save(doc,gearElem);
}
*/
void Gear::unSynch()
{
  _parentSchema->unSynch();
}

void Gear::prePlay()
{
  internalPrePlay();
}

void Gear::postPlay()
{
  internalPostPlay();
}

void Gear::init()
{
  qDebug() << "init gear: " << _Type;

  //call the virtual method
  internalInit();
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

bool Gear::isPlugNameUnique(QString name)
{
  //check for duplicate plug name
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
  {
    if ((*it)->name() == name)
      return false;
  }

  return true;
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
  if (!plug)
    return NULL;

  if (!isPlugNameUnique(plug->name()))
    return NULL;
  /*
    //if plug is input, add only plug for main type
    if (plug->inOut()==IN)
      _plugs.push_back(plug);  
    else
      //add main plug and plugs for its subType
      addPlugAndSubPlugs(plug,0);  
  */

  _plugs.push_back(plug);

  return plug;
}

//! temp method
/*
void Gear::addPlugAndSubPlugs(AbstractPlug* plug, int level)
{
  QString str;
 
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
*/

void Gear::getInputs(QList<AbstractPlug*> &inputs, bool onlyExposed) const
{
  inputs.clear();
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
  {
    if ( ((*it)->inOut() == IN && ((onlyExposed && (*it)->exposed()) || !onlyExposed)) )
      inputs.push_back(*it);
  }
}

void Gear::getOutputs(QList<AbstractPlug*> &outputs, bool onlyExposed) const
{
  outputs.clear();
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
  {
    if ( ((*it)->inOut() == OUT) && ((onlyExposed && (*it)->exposed()) || !onlyExposed) )
      outputs.push_back(*it);
  }
}

void Gear::getAllPlugs(QList<AbstractPlug*> &plugs) const
{
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
		plugs.push_back(*it);
}

void Gear::getDependencies(std::vector<Gear*> &dependencies) const
{
  QList<AbstractPlug*> inputs;
  getInputs(inputs);
  std::list<AbstractPlug*> lplug;
  for (QList<AbstractPlug*>::ConstIterator it = inputs.constBegin();it!=inputs.constEnd();++it)
  {
    (*it)->connectedPlugs(lplug);
    if (lplug.size()!=0)
      dependencies.push_back(lplug.back()->parent());
  }
}

void Gear::save(QDomDocument &doc, QDomElement &parent)
{
  QDomElement gearElem = doc.createElement(XML_TAGNAME);
  parent.appendChild(gearElem);

  QDomAttr gearType = doc.createAttribute("Type");
  gearType.setValue(_Type);
  gearElem.setAttributeNode(gearType);

  QDomAttr gearName = doc.createAttribute("Name");
  gearName.setValue(_name);
  gearElem.setAttributeNode(gearName);

  //save plugs
  QDomElement plugElem = doc.createElement("Plugs");
  gearElem.appendChild(plugElem);
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
    (*it)->save(doc, plugElem);

  internalSave(doc, gearElem);
}

void Gear::load(QDomElement &gearElem)
{
  _name = gearElem.attribute("Name","");

  internalLoad(gearElem);

  //load plugs attributes
  QDomNode plugsNode = XMLHelper::findChildNode(gearElem, "Plugs");

  QDomNode plugNode = plugsNode.firstChild();
  while (!plugNode.isNull())
  {
    QDomElement plugElem = plugNode.toElement();
    if (!plugElem.isNull())
    {
			QString name = plugElem.attribute("Name","");
			//now find this plug and load is attributes
			for (std::list<AbstractPlug*>::iterator it=_plugs.begin(); it != _plugs.end(); ++it)
			{
				if ((*it)->name() == name)
					(*it)->load(plugElem);
			}
		}
		plugNode = plugNode.nextSibling();
	}
}

AbstractPlug* Gear::getInput(QString name) const
{
  QList<AbstractPlug*> inputs;
  getInputs(inputs);
  
  QString nameAlower=name.toLower();
  
  for (QList<AbstractPlug*>::ConstIterator it = inputs.constBegin();it!=inputs.constEnd();++it)
  {    
    if (nameAlower == (*it)->name().toLower())
      return(*it);
  }

  return NULL;
}

AbstractPlug *Gear::getPlug(QString name) const
{
  QString nameAlower=name.toLower();
  
  for (std::list<AbstractPlug*>::const_iterator it = _plugs.begin();it!=_plugs.end();++it)
  {    
    if (nameAlower == (*it)->name().toLower())
      return(*it);
  }

  return NULL;
}

AbstractPlug* Gear::getOutput(QString name) const
{
  QList<AbstractPlug*> outputs;
  getOutputs(outputs);

  int (*pf)(int)=tolower;
  QString nameAlower=name.toLower();
  
  for (QList<AbstractPlug*>::ConstIterator it = outputs.constBegin();it!=outputs.constEnd();++it)
  {
    if (nameAlower == (*it)->name().toLower())
      return(*it);
  }

  return NULL;
}

/**
 * Is gear ready to be processed.
 * A gear is ready when all plugs are ready.
 */
void Gear::evaluateReady()
{
  bool _oldStatus = _ready;
  _ready=true;

  //manage the "at least one of them connected mechanism" by setting connected plugs
  //in the vector to mandatory and the others to not mandatory
  if (_atLeastOneOfThemNeeded.size()>0)
  {
    bool atLeastOneConnected=false;
    for(std::vector<AbstractPlug*>::iterator it=_atLeastOneOfThemNeeded.begin();it!=_atLeastOneOfThemNeeded.end();++it)
    {
      if ((*it)->connected())
      {
        atLeastOneConnected=true;
        (*it)->mandatory(true);
      }
      else
        (*it)->mandatory(false);
    }

    if (!atLeastOneConnected)
    {
      _ready=false;
      if (_ready!=_oldStatus)
        emit readyStatusChanged();
      return;
    }
  }


  QList<AbstractPlug*> inputs;
  getInputs(inputs);
  for(QList<AbstractPlug*>::Iterator it=inputs.begin();it!=inputs.end();++it)
  {
    if (!(*it)->ready())
    {
      _ready=false;
      if (_ready!=_oldStatus)
        emit readyStatusChanged();
      return;
    }
  }

  QList<AbstractPlug*> outputs;
  getOutputs(outputs);
  for(QList<AbstractPlug*>::Iterator it=outputs.begin();it!=outputs.end();++it)
  {
    if (!(*it)->ready())
    {
      _ready=false;
      if (_ready!=_oldStatus)
        emit readyStatusChanged();
      return;
    }
  }

  if (_ready!=_oldStatus)
    emit readyStatusChanged();
}

void Gear::setPlugAtLeastOneNeeded(std::vector<AbstractPlug*> &plugs)
{
  _atLeastOneOfThemNeeded = plugs;
}

