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

class Gear;

class AbstractPlug  
{
public:
  AbstractPlug(Gear* parent, eInOut inOut, std::string name, AbstractType* type);
  virtual ~AbstractPlug();

  virtual void init(){};
  bool canStartConnection();
  bool canConnect(AbstractPlug *plug, bool onlyTypeCheck=false);
  bool connect(AbstractPlug *plug);
  bool disconnect(AbstractPlug *plug);
  void disconnectAll();

  bool connected() const { return !_connectedPlugs.empty();};

  virtual void onConnection(AbstractPlug*){};//!overloader pour ajouter fonctionnalites APRES une bonne connection
  virtual void onDisconnection(AbstractPlug*){};//!overloader pour ajouter fonctionnalites AVANT deconnection

  AbstractType* abstractType() const { return _abstractType;}
  eInOut inOut() const {return _inOut;};

  int connectedPlugs(std::list<AbstractPlug*> &connectedplugs) const;
  AbstractPlug* firstConnectedPlug(){return _connectedPlugs.front();} 
  int nbConnectedPlugs() const {return _connectedPlugs.size();};
  Gear* parent() const {return _parent;};

  std::string fullName() const;
  std::string shortName(int nbChars) const;
  std::string name() const {return _name;};

protected:
  std::list<AbstractPlug*> _connectedPlugs;    

private:
  Gear *_parent;
  eInOut _inOut;
  std::string _name;
  AbstractType *_abstractType;

};

#endif //__ABSTRACTPLUG_INCLUDED
