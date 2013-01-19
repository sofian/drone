#ifndef INC_GEARINFO
#define INC_GEARINFO

#include <qstring.h>
#include <qmap.h>
#include <vector>
#include "XMLHelper.h"
#include "AbstractPlug.h"
#include "../contrib/frei0r/spec/frei0r.h"
#include "Control.h"

class Gear;
class Schema;

/**
 * Pluginfo provide the meta information associated with each plug of the gear. PlugInfo are saved with GearInfo.
 *
**/ 
class PlugInfo
{
public:
	PlugInfo(){}
  PlugInfo(QString name, eInOut inOut, QString type, QString help="") :
	  _name(name),
		_inOut(inOut),
		_type(type),
		_help(help)
		{}

	QString name() const {return _name;}
	void name(QString v){_name=v;}

	eInOut inOut() const {return _inOut;}
	void inOut(eInOut v){_inOut=v;}

	QString type() const {return _type;}
	void type(QString v){_type=v;}

	QString help() const {return _help;}
	void help(QString v){_help=v;}

	bool save(QDomDocument doc, QDomElement parent)  const;
	bool load(QDomElement elem);
		
private:		
	QString _name;
	eInOut _inOut;
  QString _type;
  QString _help;
};

/**
 * GearInfo provide every information related to a Gear plugin and is actually used to instanciate
 * gears. GearInfo manage the plugin file and the metaInfo file.
 *
 * Each type of drone plugins inherit GearInfo. The createGearInstance(), infoFile() and bindPlugin() method must be implemented.
**/ 
class GearInfo
{
public:
  typedef Gear* (*GearCreator)();
  typedef Control* (*ControlCreator)();

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
	virtual QString type();//! name of the gearInfo is taken from the infoFile name.
	QString fullType(){return "Gear:"+pluginType() + ":" + type();}//! the type + ":" + the name
	virtual Gear* createGearInstance()=0;

  bool setPlugInfo(const PlugInfo &plugInfo);
	PlugInfo getPlugInfo(QString name) {return _plugsInfo.value(name);}
	QMap<QString, PlugInfo> getAllPlugsInfo(){return _plugsInfo;}
  const QStringList getClassification(){return _classification;}
  
protected:
    
  
  void syncPlugsInfo();
	bool addPlugInfo(const PlugInfo& pi);
	
  virtual bool loadMetaInfo();

	static const QString XML_TAGNAME;

	QString _pluginType;
  QMap<QString, PlugInfo> _plugsInfo;
	int _majorVersion;
  int _minorVersion;
  QString _author,_intro,_description;
  QStringList _classification;
  bool _instanciableFromGUI;
};







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
  GearCreator _makeGear;	
};


/**
 * GearInfo for static drone gears. (Gears that are bundled in app)
**/
class GearInfoStatic : public GearInfo
{
public:
	static const QString TYPENAME;
	
	GearInfoStatic(QString type, GearCreator _makeGear);
	~GearInfoStatic();	
	Gear* createGearInstance();
  QString type();//! overriden because no plugin file 
	
	QFileInfo infoFile()
	{
		return QFileInfo();//_pluginFile.absolutePath() + "/" + _pluginFile.baseName().mid(3) + ".xml");
	}
					
protected:
  GearCreator _makeGear;
  QString _type;
};



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

  ControlCreator _makeControl;	

	void* _handle;

};


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

#endif
