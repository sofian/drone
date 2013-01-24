/* 
 * File:   GearInfoDrone.h
 * Author: julien
 *
 * Created on January 20, 2013, 8:34 PM
 */

#ifndef GEARINFODRONE_H
#define	GEARINFODRONE_H

#include "GearInfoPlugin.h"
#include "GearMaker.h"
/**
 * GearInfo for drone gears.
**/
class GearInfoDrone : public GearInfoPlugin
{
public:
	static const QString TYPENAME;
	
	GearInfoDrone(QFileInfo pluginFile);
	~GearInfoDrone();	
	Gear* createGearInstance();

	QFileInfo infoFile()
	{
		return QFileInfo(_pluginFile.absolutePath() + "/" + _pluginFile.baseName().mid(3) + ".xml");
	}
					
protected:
	bool bindPlugin();

	void* _handle;
  GearMaker::GearCreator _makeGear;	
};



#endif	/* GEARINFODRONE_H */

