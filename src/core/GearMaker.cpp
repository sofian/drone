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
#include "Gear_Frei0r.h"
#include "frei0r.h"
#include "error.h"
#include <qdir.h>
#include <iostream>
#include <dlfcn.h>

#if defined(Q_OS_MACX)
#include <CFBundle.h>
#endif

GearMaker GearMaker::_registerMyself;
std::map<std::string, GearMaker::GearPluginDefinition*> *GearMaker::_registry;

void* GearMaker::_frei0rHandle;

GearMaker::GearMaker() 
{  
  _registry = new std::map<std::string, GearMaker::GearPluginDefinition*>;
  parseGears();
}

GearMaker::~GearMaker()
{
  //delete GearPluginDefinitions
  for(std::map<std::string, GearMaker::GearPluginDefinition*>::iterator it=_registry->begin(); it!=_registry->end();++it)
  {
    if (it->second->pluginType() == FREI0R_PLUGIN)
      free(it->second->gearInfo().data);
    delete it->second;
  }
  
  delete _registry;
}

Gear* GearMaker::makeGear(Schema *schema, std::string type, std::string uniqueName)
{
  const char *error;
  std::map<std::string, GearMaker::GearPluginDefinition*>::iterator it = _registry->find(type);

  std::cout << "calling GearMaker::makeGear" <<std::endl;
  
  //be sure we have this gear
  if (it == _registry->end())
  {
    warningmsg("gear %s not found!", type.c_str());
    return NULL;
  }

  //todo switch on various strategy depending on gearPluginType
  if (it->second->pluginType() == FREI0R_PLUGIN)
  {
    std::cout << "building up a frei0r plugin" << std::endl;
    
    void (*setFrei0rLib)(std::string myFrei0rLib);
    
    *(void**) (&setFrei0rLib) = dlsym(_frei0rHandle, "setFrei0rLib");
    if ((error = dlerror()))
    {
      std::cout<< "fuck: " <<std::endl;
      std::cout << error << std::endl;
    }

    std::cout << "frei0rlib path = " <<(char*)it->second->gearInfo().data << std::endl;
    (*setFrei0rLib)((char*)it->second->gearInfo().data);
  }
  
  //make the gear
  return it->second->makeGear(schema, uniqueName);  
}

void GearMaker::getAllGearsInfo(std::vector<const GearInfo*> &gearsInfo)
{
  // XXX TODO
  for (std::map<std::string, GearMaker::GearPluginDefinition*>::iterator it=_registry->begin(); it != _registry->end(); ++it)
  {
    gearsInfo.push_back(&(it->second->gearInfo()));
  }
}

void GearMaker::parseGears()
{           
  std::cout << "--- loading gears ---" << std::endl;
#if defined(Q_OS_MACX)
	//on osx we have to first find the bundle full path
	CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
  CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef, kCFURLPOSIXPathStyle);
	//gears are in /Contents/PlugIns
	QString qstrMacPath = QString(CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding())) + "/Contents/PlugIns";
	QDir dir(qstrMacPath);
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
  const char* error;

  GearMaker::initFrei0r();
  
  while ((fileInfo = it.current()) != 0 )
  {
    std::cout << fileInfo->fileName() << std::endl;
    
    //reset error
    dlerror();
    
    //open file
    void *handle = dlopen(fileInfo->filePath(), RTLD_LAZY);
    if (handle == _frei0rHandle)
    {
      ++it;//next file
      continue;
    }
    
    if (!(error = dlerror()))    
    {          
      //reset error
      dlerror();
      
      //query getGearInfo ptrfun interface
      GearInfo (*getGearInfo)(); 
      
      //query makeGear ptrfun interface
      Gear* (*makeGear)(Schema *schema, std::string uniqueName);
      *(void**) (&makeGear) = dlsym(handle, "makeGear");
      
      if (!(error = dlerror()))
      {
        //get gearInfo
        *(void**) (&getGearInfo) = dlsym(handle, "getGearInfo");
        GearInfo gearInfo = (*getGearInfo)();
         
        //build gear definition
        GearPluginDefinition *gearPluginDefinition = new GearPluginDefinition(gearInfo,
                                                                              DRONE_PLUGIN,
                                                                              handle, makeGear);  
        
        //add geardefintion to the registry
        (*_registry)[gearInfo.name]=gearPluginDefinition;//todo check for duplicates
      }
      else // might be a frei0r plugin
      {
        //check if it is a frei0r plugin
        void (*frei0rTest)();
        *(void**) (&frei0rTest) = dlsym(handle, "f0r_init");
        if (!(error = dlerror()))
        {
          std::cout << "this is a frei0r plugin with path = " << fileInfo->filePath() << std::endl;
          
          // get gear info
          void (*setFrei0rLib)(std::string myFrei0rLib);
          
          *(void**) (&setFrei0rLib) = dlsym(_frei0rHandle, "setFrei0rLib");
          if ((error = dlerror()))
          {
            std::cout<< "fuck: " <<std::endl;
            std::cout << error << std::endl;
          }
          
          (*setFrei0rLib)(fileInfo->filePath());
    
          *(void**) (&getGearInfo) = dlsym(_frei0rHandle, "getGearInfo");
          GearInfo gearInfo = (*getGearInfo)();
          std::cout << "got info" <<std::endl;
          
          // XXX TODO: should be a handle to Gear_Frei0r
          *(void**) (&makeGear) = dlsym(_frei0rHandle, "makeGear");
          if ((error = dlerror()))
          {
            warningmsg("fail to query interfaces for gear %s!", fileInfo->fileName().ascii());
            std::cout << error << std::endl;
          }

          std::cout<< "making def" << std::endl;
          GearPluginDefinition *gearPluginDefinition = new GearPluginDefinition(gearInfo,
                                                                                FREI0R_PLUGIN,
                                                                                _frei0rHandle, makeGear);  
          
          //add geardefintion to the registry
          (*_registry)[gearInfo.name]=gearPluginDefinition;//todo check for duplicates
        }
        else
        {
          warningmsg("fail to query interfaces for gear %s!", fileInfo->fileName().ascii());
          std::cout << error << std::endl;      
        }
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

void GearMaker::initFrei0r()
{
  const char* error;
  _frei0rHandle = dlopen("gears/libGear_Frei0r.so", RTLD_LAZY);
  if ((error = dlerror()))
  {
    warningmsg("cannot load Gear_Frei0r! frei0r not supported");
    std::cout << error << std::endl;      
  }
}
