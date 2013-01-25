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

#include "DroneCore.h"
#include "Gear.h"
#include "GearMaker.h"
#include "GearFrei0r.h"
#include "GearControl.h"
#include "frei0r.h"
#include "error.h"
#include <qdir.h>
#include <qdom.h>
#include <iostream>
#include "GearInfo.h"
#include "GearInfoStatic.h"
#include "GearInfoControl.h"
#include "GearInfoFrei0r.h"
#include "GearInfoDrone.h"
#include "GearInfoMeta.h"

#if defined(Q_OS_MACX)
#include <CFBundle.h>
#endif

const QString GearMaker::DRONEGEARS_SUBPATH = "drone";
const QString GearMaker::CONTROLGEARS_SUBPATH = "controls";
const QString GearMaker::FREI0RGEARS_SUBPATH = "frei0r";
const QString GearMaker::METAGEARS_SUBPATH = "meta";


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
 * Creates an instance of "type" gear, using all parsed gearInfos.
 * The init() method of gear is also called.
 */
Gear* GearMaker::makeGear(QString type)
{
  Gear* thegear;
  
  //qDebug()<<"Trying to make gear of type : "<<type<<" with keys:"<<_registry.keys();
  
  GearInfo *gearInfo = findGearInfo(type);

  //be sure we have this gear
  if (!gearInfo)
  {
    qCritical() << "[GearMaker::makeGear] gear type not found: " << type;
    return NULL;
  }

	thegear = gearInfo->createGearInstance();
	thegear->setGearInfo(gearInfo);
  thegear->setUUID(DroneCore::newUUID());
	if (thegear)
		thegear->init();
		
	return thegear;
}

Gear* GearMaker::makeGear(QString type, QString name)
{
	return makeGear(type + ":" + name);
}

/*
GearInfo* GearMaker::findGearInfo(QString type, QString name)
{
	return findGearInfo(type + ":" + name);
}*/

/**
* Find the gearInfo using the type (ex: Drone:Blur)
**/
GearInfo* GearMaker::findGearInfo(QString type)
{
  QMap<QString, GearInfo*>::iterator it = _registry.find(type);

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
	// removed because we have static gears now that have already been registered
  //emptyRegistry();
	parseDroneGears(rootDir);
	parseControlGears(rootDir);
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

void GearMaker::registerStaticGear(GearCreator gear_creator)
{
		GearInfoStatic *gearInfo = new GearInfoStatic(gear_creator);
		if(gearInfo->load())
      _registry[gearInfo->type()]=gearInfo;
    qDebug()<<_registry.keys();
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
		if (gearInfo->load())
			_registry[gearInfo->type()]=gearInfo;
		else
		  delete gearInfo;
  }
}


void GearMaker::parseDroneGears(QDir rootDir)
{  	
  std::cout << "--- loading drone gears ---" << std::endl;

	QDir gearsDir(rootDir.path() + "/" + DRONEGEARS_SUBPATH); 
	
#if defined(Q_OS_MACX)
  	QString extension("*.dylib.1");
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

void GearMaker::parseControlGears(QDir rootDir)
{
  std::cout << "--- loading control gears ---" << std::endl;

	QDir gearsDir(rootDir.path() + "/" +CONTROLGEARS_SUBPATH); 
	
#if defined(Q_OS_MACX)
  	QString extension("*.dylib");
#else
  	QString extension("*.so");
#endif
	parseGears<GearInfoControl>(gearsDir, extension);
}

void GearMaker::parseMetaGears(QDir rootDir)
{
  std::cout << "--- loading Meta gears ---" << std::endl;

	QDir gearsDir(rootDir.path() + "/" + METAGEARS_SUBPATH); 
	
	QString extension("*.meta");

	parseGears<GearInfoMeta>(gearsDir, extension);
}

