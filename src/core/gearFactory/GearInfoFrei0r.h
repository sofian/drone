/* 
 * File:   GearInfoFrei0r.h
 * Author: julien
 *
 * Created on January 20, 2013, 8:35 PM
 */

#ifndef GEARINFOFREI0R_H
#define	GEARINFOFREI0R_H
#include "GearInfoPlugin.h"


/**
 * GearInfo for drone Frei0r gears.
**/
class GearInfoFrei0r : public GearInfoPlugin
{
public:
	static const QString TYPENAME;
	
	GearInfoFrei0r(QFileInfo pluginFile);
	~GearInfoFrei0r();				
												  
	QFileInfo infoFile()
	{
		return QFileInfo(_pluginFile.absolutePath() +  "/" + _pluginFile.baseName().mid(3) + ".xml");
	}
										
	Gear* createGearInstance();
	bool bindPlugin();
					
protected:
	bool loadMetaInfo();

	void* _handle;
	f0r_plugin_info_t _pluginInfo;

};


#endif	/* GEARINFOFREI0R_H */

