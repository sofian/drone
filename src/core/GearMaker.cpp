/* GearMaker.cpp
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

#include "Gear.h"
#include "GearMaker.h"
#include "GearFrei0r.h"
#include "frei0r.h"
#include "error.h"
#include <qdir.h>
#include <qdom.h>
#include <iostream>
#include <dlfcn.h>

#if defined(Q_OS_MACX)
#include <CFBundle.h>
#endif

GearMaker GearMaker::_registerMyself;
std::map<std::string, GearInfo*> *GearMaker::_registry;

GearMaker::GearMaker()
{
  _registry = new std::map<std::string, GearInfo_*>;
}

GearMaker::~GearMaker()
{
  emptyRegistry();
  delete _registry;
}

void GearMaker::emptyRegistry()
{
  //delete GearPluginDefinitions
  for(std::map<std::string, GearInfo_*>::iterator it=_registry->begin(); it!=_registry->end();++it)
  {
    if (it->second->gearInfo_()->pluginType() == GearInfo_::FREI0R_PLUGIN)
      free(it->second->gearInfo().data);
    delete it->second;
  }
}

Gear* GearMaker::makeGear(Schema *schema, std::string type, std::string uniqueName)
{
  Gear* thegear;
  std::map<std::string, GearInfo_*>::iterator it = _registry->find(type);

  std::cout << "calling GearMaker::makeGear: " << type << std::endl;

  //be sure we have this gear
  if (it == _registry->end())
  {
    warningmsg("gear %s not found!", type.c_str());
    return NULL;
  }

  //todo switch on various strategy depending on gearPluginType
  if (it->second->gearInfo_()->pluginType() == GearInfo_::FREI0R_PLUGIN)
  {
    std::cout << "building up a frei0r plugin" << std::endl;
    std::cout << "frei0rlib path = " <<(char*)it->second->gearInfo().data << std::endl;
    //GearFrei0r::setGlobalFrei0rLib((char*)it->second->gearInfo().data);
    thegear= GearFrei0r::makeGear(schema, uniqueName, (char*)it->second->gearInfo().data);
    thegear->_gearInfo_=it->second->gearInfo_();
    return thegear;
  }
  else
  {
    //make the gear
    thegear = it->second->makeGear(schema, uniqueName);
    thegear->_gearInfo_ = it->second->gearInfo_();
    return thegear;
  }
}

void GearMaker::getAllGearsInfo(std::vector<const GearInfo*> &gearsInfo)
{
  // XXX TODO
  for (std::map<std::string, GearInfo_*>::iterator it=_registry->begin(); it != _registry->end(); ++it)
  {
    gearsInfo.push_back(&(it->second->gearInfo()));
  }
}

void GearMaker::getAllGearsInfoWithNameFilter(std::vector<const GearInfo*> &gearsInfo, std::string filter)
{


  for (std::map<std::string, GearInfo_*>::iterator it=_registry->begin(); it != _registry->end(); ++it)
  {
    if(QString(it->second->gearInfo().name).lower().find(QString(filter.c_str()).lower()) != -1)
      gearsInfo.push_back(&(it->second->gearInfo()));
  }
}

void GearMaker::parseGears()
{
  std::cout << "--- loading gears ---" << std::endl;
  emptyRegistry();
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
        //GearInfo gearInfo = (*getGearInfo)();
        GearInfo_ gearInfo_;

        gearInfo_=loadGearInfo(dir.path()+"/"+fileInfo->baseName().mid(8)+".xml",fileInfo->baseName().mid(8),GearInfo_::DRONE_PLUGIN);
        gearInfo_._handle=handle;
        gearInfo_.makeGear=makeGear;
        //build gear definition
        //GearInfo_ *gearPluginDefinition = new GearPluginDefinition(gearInfo,gearInfo_,
        //    handle, makeGear);

        //add geardefintion to the registry
        (*_registry)[gearInfo_._name]=gearPluginDefinition;//todo check for duplicates
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
  //parseMetaGears();
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
      //GearInfo gearInfo = GearFrei0r::getGearInfo(fileInfo->filePath());
      GearInfo_ gearInfo_;

      gearInfo_=loadGearInfo(dir.path()+"/"+fileInfo->baseName().mid(3)+".xml",fileInfo->baseName().mid(3),GearInfo_::FREI0R_PLUGIN,gearInfo);

      // build plugin definition
      GearPluginDefinition *gearPluginDefinition = new GearPluginDefinition(gearInfo,gearInfo_,
          0, 0);
      //add geardefintion to the registry
      (*_registry)[gearInfo_._name]=gearPluginDefinition;//todo check for duplicates
    }
    else
    {
      warningmsg("fail to load gear %s!", fileInfo->fileName().ascii());
      std::cout << error << std::endl;
    }

    ++it;//next file
  }
}

/**
 * parse relative to metagears path
 */
void GearMaker::parseMetaGears()
{
  std::cout << "--- parsing MetaGears ---" << std::endl;
  emptyRegistry();

  QDir dir("metagears");

  parseMetaGearsSubDirs(dir);
}

void GearMaker::parseMetaGearsSubDirs(QDir dir)
{
  if (!dir.exists())
    return;

  dir.setFilter(QDir::All | QDir::Hidden | QDir::NoSymLinks);
  dir.setNameFilter("*.meta");
  dir.setMatchAllDirs(true);

  const QFileInfoList *files = dir.entryInfoList();
  QFileInfoListIterator it(*files);
  QFileInfo *fileInfo;

  while ((fileInfo = it.current()) != 0 )
  {
    if (fileInfo->isDir() && (fileInfo->fileName()!=".") && (fileInfo->fileName()!=".."))
      parseMetaGearsSubDirs(QDir(fileInfo->filePath()));

    if (fileInfo->isFile())
    {
      GearInfo_ gearInfo_;
      gearInfo_=loadGearInfo(fileInfo->filePath(),fileInfo->baseName(),GearInfo_::DRONE_METAGEAR,GearInfo());

      // build plugin definition
      GearPluginDefinition *gearPluginDefinition = new GearPluginDefinition(GearInfo(),gearInfo_,
          0, 0);
      //add geardefintion to the registry
      (*_registry)[gearInfo_._name]=gearPluginDefinition;//todo check for duplicates

    }

    ++it;
  }
}

GearInfo_ GearMaker::loadGearInfo(QString filename, QString fallbackname, GearInfo_::eGearPluginType type, GearInfo oldgi)
{
  QDomDocument doc("GearInfo");
  GearInfo_ gi;
  gi.createIfNotExists(filename,fallbackname,oldgi);

  QFile file(filename);
  if( !file.open( IO_ReadOnly ) )
  {
    std::cout << "GearMaker:loadGearInfo : could not open file " << filename << std::endl;
    return GearInfo_();
  }
  QString errMsg;
  int errLine;
  int errColumn;
  if (!doc.setContent(&file, true, &errMsg, &errLine, &errColumn))
  {
    std::cout << "GearMaker:loadGearInfo : parsing error in " << filename << std::endl;
    std::cout << errMsg.ascii() << std::endl;
    std::cout << "Line: " <<  errLine << std::endl;
    std::cout << "Col: " <<  errColumn << std::endl;
    file.close();
    return GearInfo_();
  }

  file.close();

  QDomNode node = doc.firstChild();

  gi.load(node,filename);
  if(gi._name=="")
    gi._name=fallbackname;
  gi._pluginType=type;
  return gi;
}

void GearMaker::createMissingGearInfoPlugHelp(GearInfo_* gi)
{
  std::cerr<<"creating gear of type "<<gi->_name<<" to sync gearInfo"<<std::endl;
  Gear* mygear = GearMaker::makeGear(NULL,gi->_name, "temp");
  gi->syncWithGear(mygear);
  delete mygear;
  std::cerr<<"just deleted "<<gi->_name<<" to sync gearInfo"<<std::endl;
}

void GearMaker::saveDefinition(GearInfo_* gi)
{
  QString t;
  if(gi->_pluginType==GearInfo_::DRONE_METAGEAR)
	t="MetaGear";
else
	t="GearInfo";

  QDomDocument doc(t);
  
  Gear* mygear = GearMaker::makeGear(NULL,gi->_name, "temp");
  mygear->saveDefinition(doc);  
  delete mygear;
  //save to file  
    std::cout << "garMaker::saveDefinition : saving in " << gi->_filename<<std::endl;


  QFile file(gi->_filename);
  if (file.open(IO_WriteOnly))
file.remove();

  if (file.open(IO_WriteOnly))
  {
    QTextStream stream(&file);
    doc.save(stream,4);
    file.close();
  }
  else
  {
    std::cout << "file io error, cannot save!" << std::endl;
    return;
  }

  return;
  
}
