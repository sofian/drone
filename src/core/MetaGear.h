#ifndef METAGEAR_INCLUDED
#define METAGEAR_INCLUDED

#include "Gear.h"
#include "Schema.h"
#include "Project.h"

#include <QMap>

class MetaGear : public Gear
{
public:  
  MetaGear();
  virtual ~MetaGear();
  virtual Schema* getInternalSchema(){return _schema;}
  
//  GearKind kind() const {return METAGEAR;}

  static const QString TYPE;
  static const QString EXTENSION;
  
  void save(QString filename);
  bool load(QString filename);
  
  void createPlugs();

  QString fullPath(){return _fullPath;}
  
protected:
  
  void internalSave(QDomDocument &doc, QDomElement &parent);
  void internalLoad(QDomElement &parent, Drone::LoadingModeFlags lmf);
    	
  Schema *_schema;
  
  QString _metaGearName;
  QString _fullPath;

  static const QColor METAGEAR_COLOR;
	
  QMap<AbstractPlug*, AbstractPlug*> _plugMapping;//!mapping between our exposed cloned plug and the real internal plug
};

#endif
