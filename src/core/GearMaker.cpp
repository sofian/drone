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
#include <iostream.h>
#include <dlfcn.h>

GearMaker GearMaker::_registerMyself;
std::map<std::string, void*> *GearMaker::_registry;

GearMaker::GearMaker() 
{  
  _registry = new std::map<std::string, void*>;
  parseGears();
}

Gear* GearMaker::makeGear(Engine *engine, std::string type, std::string name)
{    
  std::map<std::string, void*>::iterator it = _registry->find(type);

  //be sure we have this gear
  if (it == _registry->end())
  {
    warningmsg("gear %s not found!", type.c_str());
    return NULL;
  }
  
  //query fonction ptr to make gear
  void *handle = it->second;

  dlerror();

  Gear* (*makeGear)(Engine* engine, std::string name);   
  *(void**) (&makeGear) = dlsym(handle, "makeGear");

  char* error = dlerror();
  if (error)
  {
    warningmsg("fail to make gear %s!", type.c_str());
    std::cout << "dlsym error: " << error << std::endl;    
    return NULL;
  }

  //make the gear
  return (*makeGear)(engine, name);  
}

void GearMaker::getAllGearsName(std::vector<std::string> &gearsName)
{
  for (std::map<std::string, void*>::iterator it=_registry->begin(); it != _registry->end(); ++it)
  {
    gearsName.push_back(it->first);
  }
}

void GearMaker::parseGears()
{           
  std::cout << "--- loading gears ---" << std::endl;
  QDir dir("gears");
    
  if (!dir.exists())
  {
    warningmsg("no gears path");
    return;
  }
    
  dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
  dir.setNameFilter("*.so*");
  
  const QFileInfoList *files = dir.entryInfoList();
  QFileInfoListIterator it(*files);
  QFileInfo *fileInfo;
  char* error;

  while ((fileInfo = it.current()) != 0 )
  {
    std::cout << fileInfo->fileName() << " ..." << std::endl;
    
    //reset error
    dlerror();
    
    //open file
    void *handle = dlopen(fileInfo->filePath(), RTLD_LAZY);
        
    if ((error = dlerror()))    
    {
      warningmsg("fail to load gear %s!", fileInfo->fileName().ascii());
      std::cout << error << std::endl;      
    }
          
    //reset error
    dlerror();
    
    //query gearinfo
    GearInfo (*getGearInfo)();   
    *(void**) (&getGearInfo) = dlsym(handle, "getGearInfo");
    
    if ((error = dlerror()))    
    {
      warningmsg("fail to query info for gear %s!", fileInfo->fileName().ascii());
      std::cout << error << std::endl;      
    }

    GearInfo gearInfo = (*getGearInfo)();
    
    //todo check for duplicates
    //add gear to the registry
    (*_registry)[gearInfo.name]=handle;    
    ++it;
  }
}
