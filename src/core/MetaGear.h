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
  
  void save(QDomDocument &doc, QDomElement &parent);
  void load(QDomElement &parent);
  
  void createPlugs();

protected:
  GearGui* createGearGui(QCanvas *canvas);
  
  void onPlugConnected(AbstractPlug* ourPlug, AbstractPlug* otherPlug);
  void onPlugDisconnected(AbstractPlug* ourPlug, AbstractPlug* otherPlug);
	
  Schema _schema;
  
  std::string _metaGearName;
  
  static const QColor METAGEAR_COLOR;
	
	std::map<AbstractPlug*, AbstractPlug*> _plugMapping;//!mapping between our exposed cloned plug and the real internal plug
};

#endif
