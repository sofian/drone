#ifndef SCHEMA_INCLUDED
#define SCHEMA_INCLUDED

#include <QtXml>
#include <QObject>
#include <QMap>
#include <QPair>

#include "Connection.h"
#include "Project.h"

class MetaGear;
class Gear;
class AbstractPlug;
class Engine; 


class Schema : public QObject
{
Q_OBJECT
public:
  static const QString XML_TAGNAME;

  class ScheduledConnection
  {
  public:
    ScheduledConnection(AbstractPlug &a, AbstractPlug &b) : _a(&a), _b(&b) {}    
    AbstractPlug* _a;
    AbstractPlug* _b;    
  };

  Schema(MetaGear &parentMetaGear);
  virtual ~Schema();

	/////////////////////////////////////////////////////////////////////////////
  //serialization 
	/////////////////////////////////////////////////////////////////////////////
  bool save(QDomDocument& doc, QDomElement &parent, bool onlySelected=false);
	bool load(QDomElement& doc, Drone::LoadingModeFlags lmf);
  
	///////////////////////////////////////////////////////////////////////////// 
	//Gears operations
	/////////////////////////////////////////////////////////////////////////////
  //! Returns a list of unordered gears, but not expanded. Metagears are left as is.
  QList<Gear*> getGears(){return _gears.values();}  
  //! Returns a list of unordered gears, but not expanded. Metagears are left as is.
  QList<QString> getGearsUUID(){return _gears.keys();}  
  //! Returns a list of expanded gears, as GetGears, but where metagears are replaced by their internal schema.
  QList<Gear*> getDeepGears() const;
  //! Returns a list of ordered gears
  QList<Gear*> getDeepOrderedReadyGears();
  void renameGear(Gear &gear, QString newName);
  bool addGear(Gear &gear);  
  bool removeDeepGear(Gear* &gear);
  Gear* getGearByUUID(QString uuid) const;
  QList<Gear*> getGearsByUUID(QList<QString> uuids) const;
  void clear();

	///////////////////////////////////////////////////////////////////////////// 
	//Connections
	/////////////////////////////////////////////////////////////////////////////
  QList<Connection*> getAllConnections();
  bool connect(AbstractPlug &plugA, AbstractPlug &plugB);
  bool connect(Connection &connection);
  bool disconnect(AbstractPlug &plugA, AbstractPlug &plugB);
  void disconnectAll(AbstractPlug &plugA);
  
	///////////////////////////////////////////////////////////////////////////// 
	//Schema/Sub-Schema operations
	/////////////////////////////////////////////////////////////////////////////  
  QList<Schema*> getSubSchemas();
  Schema *getParentSchema();
    
	///////////////////////////////////////////////////////////////////////////// 
	//Thread-safety
	/////////////////////////////////////////////////////////////////////////////	
	void lock(){_locked=true;}
  void unlock();
  
	//TODO:wtf?	
  void unSynch(){_needSynch=true;}  
			  	
signals:
	void gearAdded(Schema &schema,Gear &gear);
	void gearRemoved(Schema &schema,Gear &gear);
	void gearRenamed(Schema &schema,Gear &gear);
	void connectionCreated(Schema &schema, Connection connection);
	void connectionRemoved(Schema &schema, Connection connection);
					
private:

//	void onGearAdded(Gear &gear);
//	void onGearRemoved(Gear &gear);

  bool needSynch();
  void synch();
  bool _needSynch;

  // maps UUIDs with Gear*
	QMap<QString, Gear*> _gears;
  QList<Gear*> _lastDeepOrderedReadyGears;
  MetaGear *_parentMetaGear;
  
	//thread safety
  bool _locked;
  QList<ScheduledConnection> _scheduledConnections;
  QList<ScheduledConnection> _scheduledDisconnections;
  QList<Gear*> _scheduledDeletes;
};

#endif
