/* AbstractPlug.h
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

#ifndef __ABSTRACTPLUG_INCLUDED
#define __ABSTRACTPLUG_INCLUDED

#include "Engine.h"
#include "AbstractType.h"
#include <iostream> // XXX debug
#include <vector>
#include <string>
#include <list>


enum eInOut
{
  IN, OUT
};

enum ePlugState
{
  ACTIVE, SLEEPING
};

class Gear;

class AbstractPlug  
{
public:
  AbstractPlug(Gear* parent, eInOut inOut, std::string name, const AbstractType* type);
  virtual ~AbstractPlug();

  virtual void init(){};
  bool canStartConnection();
  bool canConnect(AbstractPlug *plug, bool onlyTypeCheck=false);
  bool connect(AbstractPlug *plug);
  bool disconnect(AbstractPlug *plug);
  void disconnectAll();

  bool connected() const { return !_connectedPlugs.empty();};
  //bool active() const { return plugState()==ACTIVE;};

  virtual void onConnection(AbstractPlug*){};//!overloader pour ajouter fonctionnalites APRES une bonne connection
  virtual void onDisconnection(AbstractPlug*){};//!overloader pour ajouter fonctionnalites AVANT deconnection

  const AbstractType* abstractType() const { return _abstractType;}
  const AbstractType* abstractDefaultType() const { return _abstractInternalType;}
  const AbstractType* abstractHintType() const { return _abstractInternalType;}
  eInOut inOut() const {return _inOut;};

  int connectedPlugs(std::list<AbstractPlug*> &connectedplugs) const;
  AbstractPlug* firstConnectedPlug(){return _connectedPlugs.front();} 
  int nbConnectedPlugs() const {return _connectedPlugs.size();};
  Gear* parent() const {return _parent;};

  std::string fullName() const;
  std::string shortName(int nbChars) const;
  std::string name() const {return _name;};

  //virtual ePlugState plugState()=0;

protected:
  std::list<AbstractPlug*> _connectedPlugs;    
  const AbstractType *_abstractType;
  const AbstractType *_abstractInternalType;

private:
  Gear *_parent;
  eInOut _inOut;
  std::string _name;
};

#endif //__ABSTRACTPLUG_INCLUDED
