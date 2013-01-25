/* 
 * File:   GearInfoFrei0r.cpp
 * Author: julien
 * 
 * Created on January 20, 2013, 8:35 PM
 */

#include "GearInfoFrei0r.h"
#include "GearFrei0r.h"
#include <dlfcn.h>


const QString GearInfoFrei0r::TYPENAME = "Frei0r";


GearInfoFrei0r::GearInfoFrei0r(QFileInfo pluginFile) :
	GearInfoPlugin(TYPENAME, pluginFile),
	_handle(0)
{
}

GearInfoFrei0r::~GearInfoFrei0r()
{
	if (_handle)
		dlclose(_handle);
}

bool GearInfoFrei0r::bindPlugin()
{
  qDebug() << "binding: " << _pluginFile.fileName();
	
	_handle = dlopen(_pluginFile.absoluteFilePath().toAscii(), RTLD_LAZY);

	if (!_handle)
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : " << dlerror();
		return false;
	}
	
  void (*f0r_get_plugin_info)(f0r_plugin_info_t*);
  *(void**) (&f0r_get_plugin_info) = dlsym(_handle, "f0r_get_plugin_info");
	
	if (dlerror())
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : not a Frei0r plugin!";
		return false;
	}
	
  (*f0r_get_plugin_info)(&_pluginInfo);

	_majorVersion = _pluginInfo.major_version;
  _minorVersion = _pluginInfo.minor_version;
	_author = QString::fromLocal8Bit(_pluginInfo.author, strlen(_pluginInfo.author)) ;
	
	return true;
}

bool GearInfoFrei0r::loadMetaInfo()
{
	if (!GearInfo::loadMetaInfo())
		return false;
	
	//overwrite field already specified in f0r_plugin_info_t
	_majorVersion = _pluginInfo.major_version;
  _minorVersion = _pluginInfo.minor_version;
	_author = QString::fromLocal8Bit(_pluginInfo.author, strlen(_pluginInfo.author)) ;
	
	return true;
}

Gear* GearInfoFrei0r::createGearInstance()
{
  return new GearFrei0r(_handle);
}
