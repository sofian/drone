#ifndef METAGEAR_INCLUDED
#define METAGEAR_INCLUDED

#include "Gear.h"
#include "Schema.h"

#include <map>

class MetaGear : public Gear
{
public:  
  MetaGear(Schema *schema, std::string name, std::string uniqueName);
  virtual ~MetaGear();
  virtual Schema* getInternalSchema(){return &_schema;}

  //! is this gear a MetaGear? 
  virtual bool isMeta() const {return true;}
  bool ready();

  static const std::string TYPE;
  static const std::string EXTENSION;
  
  void save(std::string filename);
  bool load(std::string filename);

  void save(QDomDocument &doc, QDomElement &parent);
  void load(QDomElement &parent);
  
  void createPlugs();

  std::string fullPath(){return _fullPath;}
  

protected:
  GearGui* createGearGui(QCanvas *canvas);
  	
  Schema _schema;
  
  std::string _metaGearName;
  std::string _fullPath;

  static const QColor METAGEAR_COLOR;
	
  std::map<AbstractPlug*, AbstractPlug*> _plugMapping;//!mapping between our exposed cloned plug and the real internal plug
};

#endif
