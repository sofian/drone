/* 
 * File:   GearInfoStatic.cpp
 * Author: julien
 * 
 * Created on January 20, 2013, 8:35 PM
 */

#include "GearInfoStatic.h"
#include "GearMaker.h"

const QString GearInfoStatic::TYPENAME = "Drone";

/**
* GearInfo for drone gears.
**/
GearInfoStatic::GearInfoStatic(GearMaker::GearCreator gear_creator) :
	GearInfo(TYPENAME),
	_makeGear(gear_creator)
{
  qDebug()<<"Registering static drone gear: "<<type();
}

GearInfoStatic::~GearInfoStatic()
{
}

Gear* GearInfoStatic::createGearInstance()
{
	if (_makeGear)
		return _makeGear();
	else
		return NULL;
}

QFileInfo GearInfoStatic::infoFile()
{
  
  return QFileInfo(GearMaker::instance()->defaultGearsDir().absolutePath () + "/" + GearMaker::DRONEGEARS_SUBPATH + _type.split(":").last() +".xml");
}