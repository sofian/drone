// GearMaker.cpp: implementation of the GearMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "GearMaker.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

std::map<std::string, GearMaker*> *GearMaker::_Registry;

GearMaker::GearMaker(std::string type) 
{
    static bool registry_instanciated=false;

    if (!registry_instanciated)
    {           
        _Registry = new std::map<std::string, GearMaker*>();
        registry_instanciated=true;
    }
    
    _Registry->insert(std::make_pair(type, this));
}

Gear* GearMaker::makeGear(Engine *engine, std::string type,std::string name)
{    
    GearMaker *Gearmaker= _Registry->find(type)->second;

    //Gear non trouve dans le registry
    assert(Gearmaker != NULL);

    return Gearmaker->internalMakeGear(engine,name);
}

void GearMaker::getAllGearsName(std::vector<std::string> &gearsName)
{
    for (std::map<std::string, GearMaker*>::iterator it=_Registry->begin(); it != _Registry->end(); ++it)
    {
        gearsName.push_back(it->first);
    }
}
