#ifndef METAGEAR_INCLUDED
#define METAGEAR_INCLUDED

#include "Gear.h"
#include "Schema.h"
#include "ISchemaEventListener.h"
#include <map>

class MetaGear : public Gear, public ISchemaEventListener
{
public:  
  MetaGear(Schema *parentSchema, QString name, QString uniqueName);
  virtual ~MetaGear();
  virtual Schema* getInternalSchema(){return _schema;}
  
  GearKind kind() const {return METAGEAR;}

  static const QString TYPE;
  static const QString EXTENSION;
  
  void save(QString filename);
  bool load(QString filename);
  
  void createPlugs();

  QString fullPath(){return _fullPath;}
  
  void onGearAdded(Schema *schema, Gear *gear);
  void onGearRemoved(Schema *schema, Gear *gear);

  
protected:
  
  void internalSave(QDomDocument &doc, QDomElement &parent);
  void internalLoad(QDomElement &parent);
    	
  Schema *_schema;
  
  QString _metaGearName;
  QString _fullPath;

  static const QColor METAGEAR_COLOR;
	
  std::map<AbstractPlug*, AbstractPlug*> _plugMapping;//!mapping between our exposed cloned plug and the real internal plug
};

#endif
