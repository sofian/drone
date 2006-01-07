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

#if defined(Q_OS_MACX)
#include <CFBundle.h>
#endif

GearMaker GearMaker::_registerMyself;

QMap<QString, GearInfo*> *GearMaker::_registry;
QString GearMaker::DRONEGEARS_SUBPATH = "drone";
QString GearMaker::FREI0RGEARS_SUBPATH = "frei0r";
QString GearMaker::METAGEARS_SUBPATH = "meta";


GearMaker::GearMaker()
{
  _registry = new QMap<QString, GearInfo*>();
}

GearMaker::~GearMaker()
{
  emptyRegistry();
  delete _registry;
}

void GearMaker::emptyRegistry()
{
  for(QMap<QString, GearInfo*>::iterator it=_registry->begin(); it!=_registry->end();++it)
    delete it.value();

  _registry->clear();
}

Gear* GearMaker::makeGear(Schema *schema, QString name, QString uniqueName)
{
  Gear* thegear;
  GearInfo *gearInfo = findGearInfo(name);

  //be sure we have this gear
  if (!gearInfo)
  {
    qCritical() << "gear not found: " << name;
    return NULL;
  }

	return gearInfo->createGearInstance(schema, uniqueName);
}

GearInfo* GearMaker::findGearInfo(QString name)
{
  QMap<QString, GearInfo*>::iterator it = _registry->find(name);

	if (it == _registry->end())
		return NULL;
		
	return it.value();
}

void GearMaker::getAllGearsInfo(QList<GearInfo*> &gearsInfo)
{
	gearsInfo = _registry->values();
}

/**
* Parse every supported gears plugins located under the specified path. Once parsed, gears can be instanciated
* using GearMaker::makeGear(). The whole registry of parsed gears is cleaned up before each
* parse call.
*
* @param gearsPath the path from which you want to parse.
**/
bool GearMaker::parse(QDir rootDir)
{
	if (!rootDir.exists())
  {
    qWarning() << "gears folder not found: " << rootDir.path();
    return false;
  }
	
	qDebug() << "parsing Gears in path: " << rootDir.path();
	emptyRegistry();
	parseDroneGears(rootDir);
	parseFrei0rGears(rootDir);
	parseMetaGears(rootDir);
	
	return true;
}

/**
 * Behave has parse, but use defaultGearDir.
**/ 
bool GearMaker::parse()
{
	QDir defaultDir = defaultGearsDir();
	return parse(defaultDir);
}

QDir GearMaker::defaultGearsDir()
{
#if defined(Q_OS_MACX)
  //on osx we have to first find the bundle full path
  CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
  CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef, kCFURLPOSIXPathStyle);
  //gears are in /Contents/PlugIns
  QString qstrMacPath = QString(CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding())) + "/Contents/PlugIns";
  return QDir(qstrMacPath);
#else
  return QDir("gears");
#endif	
}


template<class T>
void GearMaker::parseGears(QDir dir, QString extension)
{
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	if (!dir.exists())
  {
    qWarning() << "gears folder not found: " << dir.path();
    return;
  }
	
  dir.setNameFilters(QStringList(extension));

  const QFileInfoList files = dir.entryInfoList();
  const char* error;

  for(QFileInfoList::const_iterator it=files.begin();it!=files.end();++it)
  {		
		T *gearInfo = new T(*it);
		//add gearInfo to the registry
		if (gearInfo->load())
			(*_registry)[gearInfo->name()]=gearInfo;//todo check for duplicates
		else
		  delete gearInfo;
    
    ++it;//next file
  }
}

void GearMaker::parseDroneGears(QDir rootDir)
{  	
  std::cout << "--- loading drone gears ---" << std::endl;

	QDir gearsDir(rootDir.path() + "/" + DRONEGEARS_SUBPATH); 
	
#if defined(Q_OS_MACX)
  	QString extension("*.dylib*");
#else
  	QString extension("*.so*");
#endif
	parseGears<GearInfoDrone>(gearsDir, extension);
}

void GearMaker::parseFrei0rGears(QDir rootDir)
{
  std::cout << "--- loading frei0r gears ---" << std::endl;

	QDir gearsDir(rootDir.path() + "/" +FREI0RGEARS_SUBPATH); 
	
#if defined(Q_OS_MACX)
  	QString extension("*.dylib*");
#else
  	QString extension("*.so*");
#endif
	parseGears<GearInfoFrei0r>(gearsDir, extension);
}

void GearMaker::parseMetaGears(QDir rootDir)
{
  std::cout << "--- loading Meta gears ---" << std::endl;

	QDir gearsDir(rootDir.path() + "/" + METAGEARS_SUBPATH); 
	
	QString extension("*.meta");

	parseGears<GearInfoMeta>(gearsDir, extension);
}

/*
void GearMaker::createMissingGearInfoPlugHelp(GearInfo_* gi)
{
  std::cerr<<"creating gear of type "<<gi->_name<<" to sync gearInfo"<<std::endl;
  Gear* mygear = GearMaker::makeGear(NULL,gi->_name, "temp");
  gi->syncWithGear(mygear);
  delete mygear;
  std::cerr<<"just deleted "<<gi->_name<<" to sync gearInfo"<<std::endl;
}
*/
/*
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
    std::cout << "gearMaker::saveDefinition : saving in " << gi->_filename<<std::endl;


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
*/