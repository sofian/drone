/* GearMaker.cpp
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

// GearMaker.cpp: implementation of the GearMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "GearMaker.h"
#include "error.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


std::map<std::string, GearMaker*> *GearMaker::_registry=NULL;

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
  if (gearMaker == NULL)
  {
      warningmsg("gear %s not found!", type.c_str());
      return NULL;
  }
  

  return gearMaker->internalMakeGear(engine,name);
}

void GearMaker::getAllGearsName(std::vector<std::string> &gearsName)
{
  for (std::map<std::string, GearMaker*>::iterator it=_registry->begin(); it != _registry->end(); ++it)
  {
    gearsName.push_back(it->first);
  }
}
