#ifndef METAGEAR_INCLUDED
#define METAGEAR_INCLUDED

#include "Gear.h"
#include "Schema.h"

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

protected:
  GearGui* createGearGui(QCanvas *canvas);
  
  Schema _schema;
  
  static const QColor METAGEAR_COLOR;
};

#endif
