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

QString GearMaker::DRONEGEARS_SUBPATH = "drone";
QString GearMaker::FREI0RGEARS_SUBPATH = "frei0r";
QString GearMaker::METAGEARS_SUBPATH = "meta";


GearMaker::GearMaker()
{
}

/**
 * Singleton instanciation not Thread-safe
*/ 
GearMaker* GearMaker::instance()
{
		static GearMaker gearMaker;
				
		return &gearMaker;
}


void GearMaker::emptyRegistry()
{
  for(QMap<QString, GearInfo*>::iterator it=_registry.begin(); it!=_registry.end();++it)
    delete it.value();

  _registry.clear();
}

/**
 * Creates an instance of fullname gear, using all parsed gearInfos.
 * The init() method of gear is also called.
 */
Gear* GearMaker::makeGear(QString fullName)
{
  Gear* thegear;
  GearInfo *gearInfo = findGearInfo(fullName);

  //be sure we have this gear
  if (!gearInfo)
  {
    qCritical() << "gear not found: " << fullName;
    return NULL;
  }

	thegear = gearInfo->createGearInstance();
	
	if (thegear)
		thegear->init();
		
	return thegear;
}

Gear* GearMaker::makeGear(QString type, QString name)
{
	return makeGear(type + ":" + name);
}

GearInfo* GearMaker::findGearInfo(QString type, QString name)
{
	return findGearInfo(type + ":" + name);
}

/**
* Find the gearInfo using the fullName (ex: Drone:Blur)
**/
GearInfo* GearMaker::findGearInfo(QString fullName)
{
  QMap<QString, GearInfo*>::iterator it = _registry.find(fullName);

	if (it == _registry.end())
		return NULL;
		
	return it.value();
}

void GearMaker::getAllGearsInfo(QList<GearInfo*> &gearsInfo)
{
	gearsInfo = _registry.values();
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
	dir.setFilter(QDir::Files | QDir::Hidden);
	if (!dir.exists())
  {
    qWarning() << "gears folder not found: " << dir.path();
    return;
  }
	
  dir.setNameFilters(QStringList(extension));

  const QFileInfoList files = dir.entryInfoList();
  const char* error;

  foreach(QFileInfo fi,files)
  {		
		T *gearInfo = new T(fi);
		//add gearInfo to the registry
		//the key will be unique since each gears type
		//are in there own path and we use fullName as the key.
		if (gearInfo->load())
			_registry[gearInfo->fullName()]=gearInfo;
		else
		  delete gearInfo;
  }
}

void GearMaker::parseDroneGears(QDir rootDir)
{  	
  std::cout << "--- loading drone gears ---" << std::endl;

	QDir gearsDir(rootDir.path() + "/" + DRONEGEARS_SUBPATH); 
	
#if defined(Q_OS_MACX)
  	QString extension("*.dylib");
#else
  	QString extension("*.so");
#endif
	parseGears<GearInfoDrone>(gearsDir, extension);
}

void GearMaker::parseFrei0rGears(QDir rootDir)
{
  std::cout << "--- loading frei0r gears ---" << std::endl;

	QDir gearsDir(rootDir.path() + "/" +FREI0RGEARS_SUBPATH); 
	
#if defined(Q_OS_MACX)
  	QString extension("*.dylib");
#else
  	QString extension("*.so");
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

