/* GearMaker.cpp
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
#include "GearMaker.h"
#include "error.h"
#include <qdir.h>
#include <iostream>
#include <dlfcn.h>

GearMaker GearMaker::_registerMyself;
std::map<std::string, GearMaker::GearPluginDefinition*> *GearMaker::_registry;

GearMaker::GearMaker() 
{  
  _registry = new std::map<std::string, GearMaker::GearPluginDefinition*>;
  parseGears();
}

GearMaker::~GearMaker()
{
  //delete GearPluginDefinitions
  for(std::map<std::string, GearMaker::GearPluginDefinition*>::iterator it=_registry->begin(); it!=_registry->end();++it)
    delete it->second;

  delete _registry;
}

Gear* GearMaker::makeGear(Schema *schema, std::string type, std::string uniqueName)
{    
  std::map<std::string, GearMaker::GearPluginDefinition*>::iterator it = _registry->find(type);

  //be sure we have this gear
  if (it == _registry->end())
  {
    warningmsg("gear %s not found!", type.c_str());
    return NULL;
  }
  
  //todo switch on various strategy depending on gearPluginType

  //make the gear
  return it->second->makeGear(schema, uniqueName);  
}

void GearMaker::getAllGearsInfo(std::vector<const GearInfo*> &gearsInfo)
{
  for (std::map<std::string, GearMaker::GearPluginDefinition*>::iterator it=_registry->begin(); it != _registry->end(); ++it)
  {
    gearsInfo.push_back(&(it->second->gearInfo()));
  }
}

void GearMaker::parseGears()
{           
  std::cout << "--- loading gears ---" << std::endl;
#if defined(Q_OS_MACX)
	QDir dir("../Gears");
#else
	QDir dir("gears");
#endif
  if (!dir.exists())
  {
    warningmsg("no gears path");
    return;
  }
    
  dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
#if defined(Q_OS_MACX)
  dir.setNameFilter("*.dylib*");
#else
  dir.setNameFilter("*.so*");
#endif  
  
  const QFileInfoList *files = dir.entryInfoList();
  QFileInfoListIterator it(*files);
  QFileInfo *fileInfo;
  char* error;

  while ((fileInfo = it.current()) != 0 )
  {
    std::cout << fileInfo->fileName() << std::endl;
    
    //reset error
    dlerror();
    
    //open file
    void *handle = dlopen(fileInfo->filePath(), RTLD_LAZY);
        
    if (!(error = dlerror()))    
    {          
      //reset error
      dlerror();
      
      //query getGearInfo ptrfun interface
      GearInfo (*getGearInfo)();   
      *(void**) (&getGearInfo) = dlsym(handle, "getGearInfo");
           
      //query makeGear ptrfun interface
      Gear* (*makeGear)(Schema *schema, std::string uniqueName);   
      *(void**) (&makeGear) = dlsym(handle, "makeGear");

      if (!(error = dlerror()))    
      {
        //get gearInfo
        GearInfo gearInfo = (*getGearInfo)();
        
        //build gear definition
        GearPluginDefinition *gearPluginDefinition = new GearPluginDefinition(gearInfo, DRONE_PLUGIN, handle, makeGear);
        
        //add geardefintion to the registry
        (*_registry)[gearInfo.name]=gearPluginDefinition;//todo check for duplicates
      }
      else
      {
        warningmsg("fail to query interfaces for gear %s!", fileInfo->fileName().ascii());
        std::cout << error << std::endl;      
      }

    }
    else
    {
      warningmsg("fail to load gear %s!", fileInfo->fileName().ascii());
      std::cout << error << std::endl;      
    }

    ++it;//next file
  }
}
