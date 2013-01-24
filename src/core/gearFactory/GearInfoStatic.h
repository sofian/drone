/* 
 * File:   GearInfoStatic.h
 * Author: julien
 *
 * Created on January 20, 2013, 8:35 PM
 */

#ifndef GEARINFOSTATIC_H
#define	GEARINFOSTATIC_H

#include "GearInfo.h"
#include "GearMaker.h"
/**
 * GearInfo for static drone gears. (Gears that are bundled in app)
**/
class GearInfoStatic : public GearInfo
{
public:
	static const QString TYPENAME;
	
	GearInfoStatic(GearMaker::GearCreator _makeGear);
	~GearInfoStatic();	
	Gear* createGearInstance();
	
	QFileInfo infoFile();
					
protected:
  GearMaker::GearCreator _makeGear;
};



#endif	/* GEARINFOSTATIC_H */

