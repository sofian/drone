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
#include "GearFrei0r.h"
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
  std::map<std::string, GearMaker::GearPluginDefinition*>::iterator it = _registry->find(type);
  
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
    std::cout << "frei0rlib path = " <<(char*)it->second->gearInfo().data << std::endl;
    //GearFrei0r::setGlobalFrei0rLib((char*)it->second->gearInfo().data);
    return GearFrei0r::makeGear(schema, uniqueName, (char*)it->second->gearInfo().data);
  }
  else
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
        warningmsg("fail to load gear %s!", fileInfo->fileName().ascii());
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

  parseFrei0rPlugins();
}

void GearMaker::parseFrei0rPlugins()
{
  std::cout << "--- loading frei0r plugins ---" << std::endl;
#if defined(Q_OS_MACX)
	//on osx we have to first find the bundle full path
	CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
  CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef, kCFURLPOSIXPathStyle);
	//gears are in /Contents/PlugIns
	QString qstrMacPath = QString(CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding())) + "/Contents/PlugIns/frei0r";
	QDir dir(qstrMacPath);
#else
	QDir dir("gears/frei0r");
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

  while ((fileInfo = it.current()) != 0 )
  {
    std::cout << fileInfo->fileName() << std::endl;
    
    //reset error
    dlerror();
    
    //open file
    dlopen(fileInfo->filePath(), RTLD_LAZY);
    
    if (!(error = dlerror()))
    {
      // get gear info
      GearInfo gearInfo = GearFrei0r::getGearInfo(fileInfo->filePath());

      // build plugin definition
      GearPluginDefinition *gearPluginDefinition = new GearPluginDefinition(gearInfo,
                                                                            FREI0R_PLUGIN,
                                                                            0, 0);
      //add geardefintion to the registry
      (*_registry)[gearInfo.name]=gearPluginDefinition;//todo check for duplicates
    }
    else
    {
      warningmsg("fail to load gear %s!", fileInfo->fileName().ascii());
      std::cout << error << std::endl;      
    }
    
    ++it;//next file
  }
}
