/* AbstractPlug.cpp
* Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#include <assert.h>
#include <iostream>
#include <sstream>
#include "AbstractPlug.h"
#include "XMLHelper.h"

#include "Gear.h"

const QString AbstractPlug::XML_TAGNAME = "Plug";
const QString AbstractPlug::XML_TAGNAME_TYPE_ELEM = "Type";

// see http://harmattan-dev.nokia.com/docs/library/html/qt4/debug.html#providing-support-for-the-qdebug-stream-operator
QDebug	operator<< ( QDebug out, const AbstractPlug & ap )
{
  QList<AbstractPlug*> connectedPlugs;
  ap.connectedPlugs(connectedPlugs);
  out << "AbstractPlug:" << ap.fullName() << " connected to ";
  foreach(AbstractPlug*p,connectedPlugs)
    out << p->fullName()<<";";
  return out.space();
  
}
 


AbstractPlug::AbstractPlug(Gear* parent, eInOut inOut, QString name, AbstractType* type, bool mandatory) :
  _abstractType(type),
  _abstractDefaultType(type),
	_mandatory(mandatory),
	_parent(parent),
  _sleeping(false),
  _inOut(inOut),
  _name(name),
  _exposed(false)
{
  //une plug a besoin d'un parent
  ASSERT_ERROR(parent!=NULL);

  if (type == NULL)
    qCritical() << Q_FUNC_INFO << " was provided with type = NULL";

  forwardPlug(0);
}

AbstractPlug::~AbstractPlug()
{
  disconnectAll();
}

bool AbstractPlug::canConnect(AbstractPlug *plug, bool onlyTypeCheck)
{
  if (plug==NULL)
    return false;

  //s'assurer que plug n'est pas deja connecte a nous
  if (_connectedPlugs.contains(plug))
    return false;

  //avons-nous bien une connection d'un in dans un out ou vice-versa
  if (_inOut == plug->inOut())
    return false;


  // Prevent downcasting.
  if (_inOut == IN)
  {
    if (! plug->abstractType()->isSubClassOf(*_abstractType))
      return false;
  }
  else
  {
    if (! plug->abstractType()->isSuperClassOf(*_abstractType))
      return false;
  }

    //assurer seulement une connection par input
  if (!onlyTypeCheck)
  {
    if (_inOut == IN)
    {
      if (!_connectedPlugs.empty())
        return false;
    } else
    {
      if (!plug->_connectedPlugs.empty())
      {
        return false;
      }
    }
  }
  return true;
}

bool AbstractPlug::connect(AbstractPlug *plug)
//! logique de connection de base
{

  if (!canConnect(plug))
    return false;

  //remove exposition
  if(this->inOut() == IN)
    exposed(false);

  if(plug->inOut() == IN)
    plug->exposed(false);

  AbstractPlug * deepestPlug = 0;
  for(deepestPlug = this; deepestPlug->_forwardPlug != 0; deepestPlug = deepestPlug->_forwardPlug) ;
  if(deepestPlug != this)
    deepestPlug->_connectedPlugs.push_back(plug);

  AbstractPlug * deepestOtherPlug = 0;
  for(deepestOtherPlug = plug; deepestOtherPlug->_forwardPlug != 0; deepestOtherPlug = deepestOtherPlug->_forwardPlug) ;
  if(deepestOtherPlug != plug)
    deepestOtherPlug->_connectedPlugs.push_back(this);

  _connectedPlugs.push_back(plug);
  plug->_connectedPlugs.push_back(this);

  _parent->onPlugConnected(this, plug);
  plug->_parent->onPlugConnected(plug, this);

  // let inherited classes do their custom logic
  onConnection(plug);
  plug->onConnection(this);

  //tell the gear that a new connection have been created and that sync is needed
  _parent->unSynch();

//   if(_forwardPlug)
//     _forwardPlug->_parent->unSynch();

  return true;
}

bool AbstractPlug::disconnect(AbstractPlug *plug)
//! logique de deconnection de base
{
  if (!plug)
    return false;

  //on ne peut pas deconnecter une plug qui n'est pas connecte a nous
  if (!_connectedPlugs.contains(plug))
  {
    qDebug()<<*this<<" has no connection to "<<*plug;
    return false;
  }
  _parent->onPlugDisconnected(this, plug);
  plug->_parent->onPlugDisconnected(plug, this);

  //laisser la chance au class derive d'executer leur logique supplementaire
  onDisconnection(plug);
  plug->onDisconnection(this);

  //remove this plug from our connections
  _connectedPlugs.removeAll(plug);
  AbstractPlug * deepestPlug = 0;
  for(deepestPlug = this; deepestPlug->_forwardPlug != 0; deepestPlug = deepestPlug->_forwardPlug) ;
  if(deepestPlug != this)
    deepestPlug->_connectedPlugs.removeAll(plug);

  //remove ourself from the other plug connections
  plug->_connectedPlugs.removeAll(this);
  AbstractPlug * deepestOtherPlug = 0;
  for(deepestOtherPlug = plug; deepestOtherPlug->_forwardPlug != 0; deepestOtherPlug = deepestOtherPlug->_forwardPlug) ;
  if(deepestOtherPlug != plug)
    deepestOtherPlug->_connectedPlugs.removeAll(this);

  //tell the gear that disconnection have been made and that we need synch
  _parent->unSynch();
//   if(_forwardPlug)
//     _forwardPlug->_parent->unSynch();

  return true;
}

void AbstractPlug::disconnectAll()
{
  while (!_connectedPlugs.empty())
  {
    disconnect(_connectedPlugs.back());
  }
}

void AbstractPlug::connectedPlugs(QList<AbstractPlug*> &connectedPlugs) const
{
	connectedPlugs = _connectedPlugs;
}

QString AbstractPlug::fullName() const
{
  return _parent->getUUID()+"/"+_name;
}

QString AbstractPlug::shortName(int nbChars) const
{
  QString abbrev;

  int c=0;
  for (QString::const_iterator it=_name.begin();it != _name.end() && c < nbChars; ++it, ++c)
  {
    abbrev+=*it;
  }

  return abbrev;
}

/**
* Change the metagear exposition status of the plug.
*
* @param exp The new metagear expose for the plug. The status will remain unchange
*            if the plug is plug <code>IN</code> type and actually connected.
*/
void AbstractPlug::exposed(bool exp)
{
  if((this->inOut() == IN) && (this->connected()))
  {
    return;
  } else
  {
    this->_exposed = exp;
  }
}

bool AbstractPlug::name(QString newName)
{
  if (!_parent->isPlugNameUnique(newName))
    return false;

  _name = newName;
  return true;
}

void AbstractPlug::save(QDomDocument &doc, QDomElement &parent) const
{
  QDomElement plugElem = doc.createElement(XML_TAGNAME);
  parent.appendChild(plugElem);

  QDomAttr nameAttr;
  nameAttr = doc.createAttribute("Name");
  nameAttr.setValue(name());
  plugElem.setAttributeNode(nameAttr);

  QDomAttr exposedAttr;
  exposedAttr = doc.createAttribute("Exposed");
  std::ostringstream oss;
  oss << exposed();
  exposedAttr.setValue(oss.str().c_str());
  plugElem.setAttributeNode(exposedAttr);

  QDomElement typeElem = doc.createElement(XML_TAGNAME_TYPE_ELEM);
  plugElem.appendChild(typeElem);
	
	if (_inOut==IN)
		_abstractDefaultType->save(doc, typeElem);
}

void AbstractPlug::load(QDomElement &plugElem)
{
  QString val = plugElem.attribute("Exposed","0");
  exposed(val == "1" ? true : false);
	
	if (_inOut==IN)
	{
		QDomNode typeNode = XMLHelper::findChildNode(plugElem, XML_TAGNAME_TYPE_ELEM);	
		if (!typeNode.isNull())
		{
			QDomElement typeElem = typeNode.toElement();
			_abstractDefaultType->load(typeElem);
		}
	}
}

void AbstractPlug::sleeping(bool s)
{
  if (s!=_sleeping)
     _parent->unSynch();

   _sleeping=s;
}
