#ifndef INC_GEARINFO
#define INC_GEARINFO

#include <qstring.h>
#include <qmap.h>
#include <vector>
#include "XMLHelper.h"
#include "AbstractPlug.h"
#include "../contrib/frei0r/spec/frei0r.h"
#include "Control.h"
#include "PlugInfo.h"

class Gear;
class Schema;

/**
 * GearInfo provide every information related to a Gear plugin and is actually used to instanciate
 * gears. GearInfo manage the plugin file and the metaInfo file.
 *
 * Each type of drone plugins inherit GearInfo. The createGearInstance(), infoFile() and bindPlugin() method must be implemented.
**/ 
class GearInfo
{
public:

  GearInfo(QString pluginType);
	virtual ~GearInfo(){}

  QString pluginType() const {return _pluginType;}
	
  virtual bool save();
  virtual bool load();	
  bool createDefaultMetaInfo();
  
  // is the gear instanciable from the Drone GUI ? 
  // (GearControl are merely hosts for controls. They are not instanciable by the user
  // The user will instead instanciate Control, which build their respective GearControl. 
  // For example, you might have 3 Controls using the same GearControl. 
  // Instanciating this gearControl doesn't make sense
  // unless you're running the core in stand alone.  
	virtual bool instanciableFromGUI(){return _instanciableFromGUI;}
  
	virtual QFileInfo infoFile()=0;//! mangling for the metaInfo filename from the pluginfile
	QString type(){return _type;}//! the type + ":" + the name
	virtual Gear* createGearInstance()=0;

  bool setPlugInfo(const PlugInfo &plugInfo);
	PlugInfo getPlugInfo(QString name) {return _plugsInfo.value(name);}
	QMap<QString, PlugInfo> getAllPlugsInfo(){return _plugsInfo;}
  const QStringList getClassification(){return _classification;}
  
protected:
  QString _type;
  
  void syncPlugsInfo();
	bool addPlugInfo(const PlugInfo& pi);
	
  virtual bool loadMetaInfo();

	static const QString XML_TAGNAME;

	QString _pluginType;
  QString _gearType;
  QMap<QString, PlugInfo> _plugsInfo;
	int _majorVersion;
  int _minorVersion;
  QString _author,_intro,_description;
  QStringList _classification;
  bool _instanciableFromGUI;
};


#endif
