/* 
 * File:   GearInfoPlugin.cpp
 * Author: julien
 * 
 * Created on January 20, 2013, 8:37 PM
 */

#include "GearInfoPlugin.h"



GearInfoPlugin::GearInfoPlugin(QString pluginType,QFileInfo pluginFile) : 
	GearInfo(pluginType),
	_pluginFile(pluginFile)
{
}


bool GearInfoPlugin::save()
{
	if (!_pluginFile.exists())
		return false;
  else 
    return GearInfo::save();
}

bool GearInfoPlugin::load()
{
	if (!_pluginFile.exists())
		return false;
  if (!bindPlugin())
		return false;

  return GearInfo::load();
}