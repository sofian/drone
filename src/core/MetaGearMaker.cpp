/* MetaGearMaker.cpp
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


#include "MetaGearMaker.h"
#include "error.h"
#include <iostream>

#if defined(Q_OS_MACX)
#include <CFBundle.h>
#endif

const std::string MetaGearMaker::METAGEAR_PATH="metagears";
MetaGearMaker MetaGearMaker::_registerMyself;
std::vector<QFileInfo*> *MetaGearMaker::_registry;

MetaGearMaker::MetaGearMaker() 
{  
  _registry = new std::vector<QFileInfo*>;
  parseMetaGears();
}

MetaGearMaker::~MetaGearMaker()
{  
  for (std::vector<QFileInfo*>::iterator it=_registry->begin();it!=_registry->end();++it)  
    delete *it;
  
  delete _registry;
}

MetaGear* MetaGearMaker::makeMetaGear(Schema *schema, std::string type, std::string uniqueName)
{
  return NULL;
}

const std::vector<QFileInfo*> &MetaGearMaker::getAllMetaGearsPath()
{
  return *_registry;
}


/**
 * parse relative to metagears path
 */
void MetaGearMaker::parseMetaGears()
{           
  std::cout << "--- parsing MetaGears ---" << std::endl;
  
  QDir dir("metagears");
        
  parseSubDirs(dir);
}

void MetaGearMaker::parseSubDirs(QDir dir)
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
      parseSubDirs(QDir(fileInfo->filePath()));
    
    if (fileInfo->isFile())
     _registry->push_back(new QFileInfo(*fileInfo));
    
    ++it;
  }
}


