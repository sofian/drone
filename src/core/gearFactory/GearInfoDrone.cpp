/* 
 * File:   GearInfoDrone.cpp
 * Author: julien
 * 
 * Created on January 20, 2013, 8:34 PM
 */

#include "GearInfoDrone.h"
#include <QtXml>
#include <qfile.h>
#include <qdir.h>
#include <QStringList>

#include <dlfcn.h>


const QString GearInfoDrone::TYPENAME = "Drone";


/**
* GearInfo for drone gears.
**/
GearInfoDrone::GearInfoDrone(QFileInfo pluginFile) :
	GearInfoPlugin(TYPENAME, pluginFile),
	_handle(0),
	_makeGear(0)
{
  qDebug();
}

GearInfoDrone::~GearInfoDrone()
{
	if (_handle)
		dlclose(_handle);
}

bool GearInfoDrone::bindPlugin()
{
	qDebug() << "binding: " << _pluginFile.fileName();

	//open file
	_handle = dlopen(_pluginFile.absoluteFilePath().toAscii(), RTLD_LAZY);

	if (!_handle)
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : " << dlerror();
		return false;
	}
	
	//query makeGear ptrfun interface
	*(void**) (&_makeGear) = dlsym(_handle, "makeGear");	
	char*e=dlerror();
	if (e)
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : not a drone plugin!. Error: '"<<e<<"'";
		return false;
	}

  return true;
}

Gear* GearInfoDrone::createGearInstance()
{
	if (_makeGear)
		return _makeGear();
	else
		return NULL;
}

