/* 
 * File:   GearInfoControl.h
 * Author: julien
 *
 * Created on January 20, 2013, 8:38 PM
 */

#ifndef GEARINFOCONTROL_H
#define	GEARINFOCONTROL_H
#include "GearInfoPlugin.h"
#include "GearMaker.h"
/**
 * GearInfo for drone control gears.
**/
class GearInfoControl : public GearInfoPlugin
{
public:
	static const QString TYPENAME;
	
	GearInfoControl(QFileInfo pluginFile);
	~GearInfoControl();				
												  
	QFileInfo infoFile()
	{
		return QFileInfo(_pluginFile.absolutePath() +  "/" + _pluginFile.baseName().mid(3) + ".xml");
	}
										
	Gear* createGearInstance();
	bool bindPlugin();
					
protected:

  GearMaker::ControlCreator _makeControl;	

	void* _handle;

};


#endif	/* GEARINFOCONTROL_H */

