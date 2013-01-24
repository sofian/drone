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
#include "gearFactory/GearInfo.h"
#include <QtXml>

#include "XMLHelper.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "BaseGearGui.h"

const QString Gear::XML_TAGNAME = "Gear";

Gear::Gear(QString type) : 
_parentSchema(NULL), 
_subType(type), 
_instanceName(type),
_gearGui(NULL),
_gearInfo(NULL),
_ready(false)
{
}

Gear::~Gear()
{
	qDeleteAll(_plugs);
}

//TODO add to new schema, remove from old schema
void Gear::parentSchema(Schema &parentSchema)
{
	_parentSchema = &parentSchema;
}

/**
 * Sets the name for the gear. If the gear is in a schema, unique naming is done via Schema::rename()
 */
void Gear::setInstanceName(QString vname)
{
	if (vname == _instanceName)
		return;

	_instanceName=vname;
	
	if (_parentSchema)
		_parentSchema->renameGear(*this, vname);				
}

QString Gear::type()
{
  if(_gearInfo!=NULL)
    return _gearInfo->type();
  else return QString();
}

void Gear::unSynch()
{
	if (_parentSchema)
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
//  qDebug() << "init gear: " << _subType;

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
  _plugs.removeAll(plug);
}

bool Gear::isPlugNameUnique(QString name)
{
  //check for duplicate plug name
  foreach(AbstractPlug* plug, _plugs)
  {
    if (plug->name() == name)
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
  foreach(AbstractPlug* plug,_plugs)
  {
    if ( (plug->inOut() == IN && ((onlyExposed && plug->exposed()) || !onlyExposed)) )
      inputs<<plug;
  }
}

void Gear::getOutputs(QList<AbstractPlug*> &outputs, bool onlyExposed) const
{
  outputs.clear();
  foreach(AbstractPlug* plug,_plugs)
  {
    if ( (plug->inOut() == OUT) && ((onlyExposed && plug->exposed()) || !onlyExposed) )
      outputs<<plug;
  }
}

void Gear::getAllPlugs(QList<AbstractPlug*> &plugs) const
{
  plugs = _plugs;
}

void Gear::getDependencies(QList<Node*> &dependencies) const
{
  QList<AbstractPlug*> inputs;
  getInputs(inputs);
	QList<AbstractPlug*> lplug;
  foreach(AbstractPlug* plug, inputs)
  {
    plug->connectedPlugs(lplug);
		foreach(AbstractPlug* plug2,lplug)
      dependencies<<plug2->parent();
  }
}

void Gear::save(QDomDocument &doc, QDomElement &parent)
{
  QDomElement gearElem = doc.createElement(XML_TAGNAME);
  parent.appendChild(gearElem);

  QDomAttr gearType = doc.createAttribute("Type");
  gearType.setValue(type());
  gearElem.setAttributeNode(gearType);

  QDomAttr gearUUID = doc.createAttribute("UUID");
  gearUUID.setValue(getUUID());
  gearElem.setAttributeNode(gearUUID);

  QDomAttr gearName = doc.createAttribute("Name");
  gearName.setValue(_instanceName);
  gearElem.setAttributeNode(gearName);

  _gearGui->save(doc, gearElem);
  _settings.save(doc, gearElem);

  //save plugs
  QDomElement plugElem = doc.createElement("Plugs");
  gearElem.appendChild(plugElem);
  foreach(AbstractPlug* plug, _plugs)
    plug->save(doc, plugElem);

  internalSave(doc, gearElem);
}

void Gear::load(QDomElement &gearElem, Drone::LoadingModeFlags lmf, bool loadUUID)               
{            
  Q_UNUSED(lmf);
  _instanceName = gearElem.attribute("Name","");
  if(loadUUID)
    _uuid = gearElem.attribute("UUID","");

  _settings.load(gearElem);  
  //updateSettings();

  internalLoad(gearElem);

  if(_gearGui)
    _gearGui->load(gearElem);

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
			foreach(AbstractPlug* plug, _plugs)
      {
				if (plug->name() == name)
					plug->load(plugElem);
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
  
  AbstractPlug* plug;
  foreach(plug,inputs)
  {    
    if (nameAlower == plug->name().toLower())
      return plug;
  }

  return NULL;
}

AbstractPlug *Gear::getPlug(QString name) const
{
  QString nameAlower=name.toLower();
  
  AbstractPlug* plug;
  foreach(plug,_plugs)
  {    
    if (nameAlower == plug->name().toLower())
      return plug;
  }

  return NULL;
}

AbstractPlug* Gear::getOutput(QString name) const
{
  QList<AbstractPlug*> outputs;
  getOutputs(outputs);

  QString nameAlower=name.toLower();
  
  AbstractPlug* plug;
  foreach(plug,outputs)
  {
    if (nameAlower == plug->name().toLower())
      return plug;
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
    foreach(AbstractPlug* plug,_atLeastOneOfThemNeeded)
    {
      if (plug->connected())
      {
        atLeastOneConnected=true;
        plug->mandatory(true);
      }
      else
        plug->mandatory(false);
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
  foreach(AbstractPlug* plug, inputs)
  {
    if (!plug->ready())
    {
      _ready=false;
      if (_ready!=_oldStatus)
        emit readyStatusChanged();
      return;
    }
  }

  QList<AbstractPlug*> outputs;
  getOutputs(outputs);
  foreach(AbstractPlug* plug, outputs)
  {
    if (!plug->ready())
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

void Gear::setPlugAtLeastOneNeeded(QList<AbstractPlug*> &plugs)
{
  _atLeastOneOfThemNeeded = plugs;
}

