/* 
 * File:   GearInfoMeta.cpp
 * Author: julien
 * 
 * Created on January 20, 2013, 8:35 PM
 */

#include "GearInfoMeta.h"
const QString GearInfoMeta::TYPENAME = "Meta";



GearInfoMeta::GearInfoMeta(QFileInfo pluginFile) :
	GearInfoPlugin(TYPENAME, pluginFile)
{
}

GearInfoMeta::~GearInfoMeta()
{
}

Gear* GearInfoMeta::createGearInstance()
{
	return NULL;
}
	
bool GearInfoMeta::bindPlugin()
{
	return true;
}
