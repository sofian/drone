// GearMaker.cpp: implementation of the GearMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "GearMaker.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

std::map<std::string, GearMaker*> *GearMaker::_registry;

GearMaker::GearMaker(std::string type) 
{
  static bool registry_instanciated=false;

  if (!registry_instanciated)
  {
    _registry = new std::map<std::string, GearMaker*>();
    registry_instanciated=true;
  }

  _registry->insert(std::make_pair(type, this));
}

Gear* GearMaker::makeGear(Engine *engine, std::string type,std::string name)
{    
  GearMaker *gearMaker= _registry->find(type)->second;

  //Gear non trouve dans le registry
  assert(gearMaker != NULL);

  return gearMaker->internalMakeGear(engine,name);
}

void GearMaker::getAllGearsName(std::vector<std::string> &gearsName)
{
  for (std::map<std::string, GearMaker*>::iterator it=_registry->begin(); it != _registry->end(); ++it)
  {
    gearsName.push_back(it->first);
  }
}
