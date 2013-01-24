/* 
 * File:   GearInfoMeta.h
 * Author: julien
 *
 * Created on January 20, 2013, 8:35 PM
 */

#ifndef GEARINFOMETA_H
#define	GEARINFOMETA_H

#include "GearInfoPlugin.h"


/**
 * GearInfo for MetaGears.
**/
class GearInfoMeta : public GearInfoPlugin
{
public:
	static const QString TYPENAME;
	
	GearInfoMeta(QFileInfo pluginFile);
	~GearInfoMeta();				

	QFileInfo infoFile()
	{
		//!!!! need to append a string like metainfo?
		return QFileInfo(_pluginFile.absolutePath() +  "/" + _pluginFile.baseName() + ".xml");
	}

	Gear* createGearInstance();

protected:
	bool bindPlugin();

};


#endif	/* GEARINFOMETA_H */

