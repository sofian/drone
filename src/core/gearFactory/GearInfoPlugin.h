/* 
 * File:   GearInfoPlugin.h
 * Author: julien
 *
 * Created on January 20, 2013, 8:37 PM
 */

#include "GearInfo.h"

#ifndef GEARINFOPLUGIN_H
#define	GEARINFOPLUGIN_H

class GearInfoPlugin: public GearInfo
{
public:
  GearInfoPlugin(QString pluginType, QFileInfo pluginFile);
	virtual ~GearInfoPlugin(){}
  virtual bool save();
  virtual bool load();	
  
protected:
    
 	virtual bool bindPlugin()=0;//! bind to the plugin file
  QFileInfo _pluginFile;
};

#endif	/* GEARINFOPLUGIN_H */

