#include <assert.h>
#include <iostream>
#include "AbstractPlug.h"

#include "Gear.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AbstractPlug::AbstractPlug(Gear* parent, eInOut inOut, std::string name, AbstractType* type) : 
  _parent(parent), 
  _inOut(inOut), 
  _name(name),
  _abstractType(type),
  _abstractInternalType(type)
{
  //une plug a besoin d'un parent
  assert(parent!=NULL);
}

AbstractPlug::~AbstractPlug()
{
  disconnectAll();
}


bool AbstractPlug::canStartConnection()
{
  //assurer seulement une connection par input
  if (_inOut == IN)
  {
    if (!_connectedPlugs.empty())
      return false;
  }

  return true;
}

bool AbstractPlug::canConnect(AbstractPlug *plug, bool onlyTypeCheck)
{
  if (plug==NULL)
    return false;

  //s'assurer que plug n'est pas deja connecte a nous
  if (find(_connectedPlugs.begin(), _connectedPlugs.end(), plug) != _connectedPlugs.end())
    return false;

  //est-ce que ce sont des plugs de meme type
  if (_abstractType->name() != plug->abstractType()->name())
    return false;

  //avons-nous bien une connection d'un in dans un out ou vice-versa
  if (_inOut == plug->inOut())
    return false;

  //assurer seulement une connection par input
  if (!onlyTypeCheck)
  {
    if (_inOut == IN)
    {
      if (!_connectedPlugs.empty())
        return false;
    } else
      if (!plug->_connectedPlugs.empty())
      return false;
  }
  return true;
}

bool AbstractPlug::connect(AbstractPlug *plug)
//! logique de connection de base
{

  if (!canConnect(plug))
    return false;

  //ajouter la nouvelle plug a nos connections
  _connectedPlugs.push_back(plug);        
  plug->_connectedPlugs.push_back(this);    

  _parent->onPlugConnected(this);
  plug->_parent->onPlugConnected(plug);

  //laisser la chance au class derive d'executer leur logique supplementaire
  onConnection(plug);
  plug->onConnection(this);

  //tell the gear that a new connection have been created and that sync is needed
  _parent->needSynch();

  return true;    
}

bool AbstractPlug::disconnect(AbstractPlug *plug)
//! logique de deconnection de base
{
  if (!plug)
    return false;

  //on ne peut pas deconnecter une plug qui n'est pas connecte a nous
  std::list<AbstractPlug*>::iterator it = find(_connectedPlugs.begin(), _connectedPlugs.end(), plug);
  if (it == _connectedPlugs.end())
    return false;

  _parent->onPlugDisconnected(this);
  plug->_parent->onPlugDisconnected(plug);

  //laisser la chance au class derive d'executer leur logique supplementaire
  onDisconnection(plug);
  plug->onDisconnection(this);

  //remove this plug from our connections
  _connectedPlugs.remove(plug);    
  //remove ourself from the other plug connections
  plug->_connectedPlugs.remove(this);

  //tell the gear that a new connection have been created and that sync is needed
  _parent->needSynch();

  return true;
}

void AbstractPlug::disconnectAll()
{
  while (!_connectedPlugs.empty())
  {
    disconnect(_connectedPlugs.back());
  }
}

int AbstractPlug::connectedPlugs(std::list<AbstractPlug*> &connectedplugs) const
{
  connectedplugs.clear();
  connectedplugs.assign(_connectedPlugs.begin(), _connectedPlugs.end());

  return connectedplugs.size();
}

std::string AbstractPlug::fullName() const
{
  return _parent->name()+"/"+_name;
}

std::string AbstractPlug::shortName(int nbChars) const
{
  std::string abbrev;

  int c=0;
  for (std::string::const_iterator it=_name.begin();it != _name.end() && c < nbChars; ++it, ++c)
  {
    abbrev+=*it;
  }

  return abbrev;
}
