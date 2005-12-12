#ifndef INC_GEARINFO
#define INC_GEARINFO
#include <qstring.h>
#include <qmap.h>
#include <vector>
#include "XMLHelper.h"
#include "oldGearInfo.h"
#include "AbstractPlug.h"

class Gear;

struct PlugInfo
{
  eInOut inOut;
  QString type;
  QString help;
};

class GearInfo_
{
  public:
  GearInfo_() : _majorVersion(1), _minorVersion(1) 
  {
    _classification.push_back("unclassified");
  }

  enum eGearPluginType {DRONE_PLUGIN, FREI0R_PLUGIN, DRONE_METAGEAR};

  void save(QDomDocument &doc,QDomNode &node);               
  bool load(QDomNode &parent,QString filename);               
  bool createIfNotExists(QString filename,QString fallbackname,GearInfo gi);
  void syncWithGear(Gear*);
  eGearPluginType pluginType() const {return _pluginType;}

 
  void saveDefinition();

  void setPlugInfo(QString name,PlugInfo*);
  int _majorVersion;
  int _minorVersion;
  QString _name,_author,_intro,_description;
  QMap<QString,PlugInfo> _plugs;
  QStringList _classification;
  QString _filename;
  static const std::string XML_TAGNAME;
  eGearPluginType _pluginType;
};

#endif
