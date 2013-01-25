#include "Schema.h"
#include "Gear.h"
#include "MetaGear.h"
#include "gearFactory/GearMaker.h"
//#include "SchemaTopoSort.h"
#include "Connection.h"
#include "BaseGearGui.h"
#include "DroneCore.h"

#include <QtXml>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include "XMLHelper.h"
#include "Project.h"

const QString Schema::XML_TAGNAME = "Schema";

Schema::Schema(MetaGear &parentMetaGear) :
_needSynch(true),  
_parentMetaGear(&parentMetaGear),
_locked(false)
{
}

Schema::~Schema()
{
  clear();
}

Gear* Schema::getGearByUUID(QString uuid) const
{
  return _gears.value(uuid, NULL);
}

QList<Gear*> Schema::getGearsByUUID(QList<QString> uuids) const
{
  QList<Gear*> ret;
  Gear* gear;
  foreach(QString uuid,uuids)
  {
    gear = _gears.value(uuid, NULL);
    if(gear)
      ret<<gear;
  }
  return ret;
}


QList<Gear*> Schema::getDeepOrderedReadyGears()
{
  if(!needSynch())
    return _lastDeepOrderedReadyGears;
/*
  QList<Gear*> deepOrderedGears;
  QList<Gear*> orderedGears;
  QList<Gear*> internalSchemaOrderedGears;

  _lastDeepOrderedReadyGears.clear();
 
  SchemaTopoSort schemaTopoSort(_gears.values());
	
  schemaTopoSort.getOrderedGears(orderedGears);

  for(QList<Gear*>::iterator it=orderedGears.begin();it!=orderedGears.end();++it)
  {
    if ((*it)->getInternalSchema()!=NULL)
    {
      internalSchemaOrderedGears.clear();
      internalSchemaOrderedGears = (*it)->getInternalSchema()->getDeepOrderedReadyGears();
      deepOrderedGears+=internalSchemaOrderedGears;
    }
    else
      deepOrderedGears+=*it;
  }


	//filter on ready gears only
	//first pass, evaluateReady for all gears
	for(QList<Gear*>::iterator it=deepOrderedGears.begin();it!=deepOrderedGears.end();++it)
	{
		(*it)->evaluateReady();
	}
		
	//finally, filter
	for(QList<Gear*>::iterator it=deepOrderedGears.begin();it!=deepOrderedGears.end();++it)
	{
		if ((*it)->ready())
			_lastDeepOrderedReadyGears.push_back(*it);
	}
  
	_needSynch=false;
*/
  return _lastDeepOrderedReadyGears;
}

QList<Gear*> Schema::getDeepGears() const
{
  QList<Gear*> deepGears;
  QList<Gear*> internalSchemaGears;

  foreach(Gear* gear, _gears)
  {
    if (gear->getInternalSchema()!=NULL)
    {
      internalSchemaGears.clear();
      internalSchemaGears = gear->getInternalSchema()->getDeepGears();
      deepGears += internalSchemaGears;  
    }
    else
      deepGears.push_back(gear);
  }

  return deepGears;
}


bool Schema::needSynch()
{
  if(_needSynch)
    return true;
  
  QList<Schema*> subschemas = getSubSchemas();
  for(QList<Schema*>::iterator it = subschemas.begin(); it!=subschemas.end();++it)
    if((*it)->needSynch())
      return true;
  
  return false;
}

void Schema::synch()
{
  _needSynch=false;
  QList<Schema*> subschemas = getSubSchemas();
  for(QList<Schema*>::iterator it = subschemas.begin(); it!=subschemas.end();++it)
    (*it)->synch();
}


void Schema::clear()
{
	qDeleteAll(_gears);
}

QList<Schema*> Schema::getSubSchemas()
{
  QList<Schema*> sub;
  foreach(Gear* gear, _gears)
    if(gear->getInternalSchema()!=NULL)
      sub += gear->getInternalSchema();
  return sub;
}

/**
 * Remove a gear from the schema, searching recursivly in subschemas
 * - ThreadSafe, if the schema is locked, operation will be performed on unlock
 * 
 * @param plugA
 * @param plugB
 * 
 * @return 
 */

bool Schema::removeDeepGear(Gear* &gear)
{
	if (_locked)
  {
    _scheduledDeletes+=gear;
    return true;
  }

  // if the gear to be removed is at the current schema level, remove it here
  if (_gears.contains(gear->getUUID()))
  {   
    emit gearRemoved(*this,*gear);
    
    _gears.remove(gear->getUUID());
            
    delete gear;
		gear=NULL;
    
    return true;
  }

  // otherwise, search recursively in subschemas
  QList<Schema*> subSchemas = getSubSchemas();
  for(QList<Schema*>::iterator it = subSchemas.begin();it!=subSchemas.end();++it)
    if((*it)->removeDeepGear(gear))
      return true;
  return false;
}

void Schema::renameGear(Gear& gear, QString newName)
{  
	if (gear.name() == newName)
		return;
}

bool Schema::addGear(Gear& gear)
{		
	//add to the schema
	_gears.insert(gear.getUUID(), &gear);
	gear.parentSchema(*this);
	emit gearAdded(*this, gear);
  return true;
}

//does not go "deep" into gears: returns only this schema's connections
// *** NOTE: the caller is responsible to delete these "dummy" helper objects
// once finished
QList<Connection*> Schema::getAllConnections()
{
  QList<AbstractPlug*> outputs;
  QList<AbstractPlug*> connectedPlugs;
  QList<Connection*> connections;

	foreach(Gear *gear, _gears)  
	{ 
    gear->getOutputs(outputs);
    foreach (AbstractPlug* sourcePlug,outputs)
    {
      sourcePlug->connectedPlugs(connectedPlugs);

      foreach(AbstractPlug* destPlug, connectedPlugs)
      {
        
        //in the case of an exposed plug, 
        //just be sure that the destination gear is really in our schema
        if ( (!sourcePlug->exposed()) || 
             (sourcePlug->exposed() && getGearByUUID(destPlug->parent()->getUUID())) )
          connections<< new Connection(gear->getUUID(), sourcePlug->name(), 
                                                destPlug->parent()->getUUID(), 
                                                destPlug->name());
      }            
    }
  }
  return connections;

}



/**
 * Connect 2 AbstractPlugs
 * - ThreadSafe, if the schema is locked, operation will be performed on unlock
 * 
 * @param plugA
 * @param plugB
 * 
 * @return 
 */
bool Schema::connect(AbstractPlug &plugA, AbstractPlug &plugB)
{  
  if (_locked)
  {
    _scheduledConnections.push_back(ScheduledConnection(plugA, plugB));
    return true;
  }
  else
  {
    bool ret = plugA.connect(&plugB);
    if(!ret)
      return false;
    Connection c(plugA.parent()->getUUID(),plugA.name(),plugB.parent()->getUUID(),plugB.name());
    emit connectionCreated(*this,c);
    return true;
  }
}

/**
 * Disconnect 2 AbstractPlugs
 * - ThreadSafe, if the schema is locked, operation will be performed on unlock
 * 
 * @param plugA
 * @param plugB
 * 
 * @return 
 */
bool Schema::disconnect(AbstractPlug &plugA, AbstractPlug &plugB)
{
  
  if (_locked)
  {
    _scheduledDisconnections.push_back(ScheduledConnection(plugA, plugB));
  }
  else
  {
    bool ret = plugA.disconnect(&plugB);
    if(!ret)
      return false;
    Connection c(plugA.parent()->getUUID(),plugA.name(),plugB.parent()->getUUID(),plugB.name());
    emit connectionRemoved(*this,c);
  }
  return true;
}

void Schema::disconnectAll(AbstractPlug &plug)
{
  
  plug.disconnectAll();
}

// this function redirects to Schema::connectin(AbstractPlug,AbstractPlug)
bool Schema::connect(Connection &connection)
{
   Gear *gearA;                                                                             
   Gear *gearB;                                                                             
   AbstractPlug *input;                                                                     
   AbstractPlug *output;                                                                    
                                                                                            
   if ( (gearA=getGearByUUID(connection.gearA())) == NULL)                                        
   {                                                                                        
     qCritical() << "connectPlugs fail: " + connection.gearA() + " not found!";  
     return false;                                                                                
   }                                                                                        
                                                                                                                                                                                        
   if ( (gearB=getGearByUUID(connection.gearB())) == NULL)                                        
   {                                                                                        
     qCritical() << "connectPlugs fail: " + connection.gearB() + " not found!";  
     return false;                                                                                
   }                                                                                        
                                                                                            
   if ( (output=gearA->getOutput(connection.output())) == NULL)                             
   {                                                                                        
     qCritical() << "connectPlugs fail: " + connection.output() + " not found!"; 
     return false;                                                                                
   }                                                                                        
                                                                                            
   if ( (input=gearB->getInput(connection.input())) == NULL)                                
   {                                                                                        
     qCritical() << "connectPlugs fail: " + connection.input() + " not found!";  
     return false;                                                                                
   }                                                                                        
                                                                                            
   return connect(*input, *output);
}

bool Schema::save(QDomDocument& doc, QDomElement &parent, bool onlySelected)
{
  QDomElement rootElem = doc.createElement(XML_TAGNAME);
  parent.appendChild(rootElem);

  //save all gears
  QDomElement gearsElem = doc.createElement("Gears");
  rootElem.appendChild(gearsElem);
  
  
	foreach(Gear *gear, _gears)
  {
    if( onlySelected && ( gear->getGearGui()==NULL || !( gear->getGearGui()->getIsSelected() ) ))
      continue;
//    std::cerr<<"About to save!"<<std::endl;
    gear->save(doc, gearsElem);            
  }

  //save all connections
  QDomElement connectionsElem = doc.createElement("Connections");
  rootElem.appendChild(connectionsElem);

  QList<Connection*> connections(getAllConnections());

  foreach(Connection* conn,connections)
  {
    Gear * gA = getGearByUUID(conn->gearA());
    Gear * gB = getGearByUUID(conn->gearB());
    if(!gA)qCritical()<<"connection save : gear not found : "<<conn->gearA();
    if(!gB)qCritical()<<"connection save : gear not found : "<<conn->gearB();
    //mute warning for now
    if(onlySelected 
       && (gA->getGearGui() == NULL || gB->getGearGui() == NULL || !gA->getGearGui()->getIsSelected()  || !gB->getGearGui()->getIsSelected()))
      continue;
    conn->save(doc, connectionsElem);

    // delete connection because "Connection" is just a temporary helper object
    delete conn;//free
  }
    
  return true;
}

bool Schema::load(QDomElement& parent, Drone::LoadingModeFlags lmf)
{    
  QSet<QString> visitedGears;
  QSet<QString> visitedConnections;
  QList<Connection*> existingConnectionsPtr(getAllConnections());
  QSet<QString> existingConnections;
  QDomNode gearsNode = XMLHelper::findChildNode(parent, "Gears");
  // if we're not merging (when pasting, gears have to be renamed
  QMap<QString,QString> renameMap;
 
  foreach(Connection* c, existingConnectionsPtr)
  {
    existingConnections<<c->toString();
    delete c;
  }
  //qDebug()<<existingConnections;
  
  if (gearsNode.isNull())
  {
    qDebug()<< "Schema::load: Bad DroneSchema : <Gears> tag not found!";
    return false;
  }

  QDomNode gearNode = gearsNode.firstChild();

  //qDebug()<<"GEARS:"<<_gears.keys();
  while (!gearNode.isNull())
  { 
    QDomElement gearElem = gearNode.toElement();
    if (!gearElem.isNull())
    {
      Gear* existingGear = getGearByUUID(gearElem.attribute("UUID", ""));
      
      // If we can UPDATE and the gear already EXISTS in schema
      if(lmf & Drone::UpdateWhenPossible && existingGear)
      {
        existingGear->load(gearElem, lmf);
        //qDebug()<<"GEARS:"<<_gears.keys();
        visitedGears<<existingGear->getUUID();
      }
      else
      {
        //we create a new gear, and change it's UUID afterwards
        Gear* newGear = GearMaker::instance()->makeGear(gearElem.attribute("Type", ""));
        if (newGear != NULL) 
        {
          QString newUUID = newGear->getUUID();
          QString oldUUID = gearElem.attribute("UUID", "");
          //addGear to schema with UUID "newUUID"
          addGear(*newGear);
          //qDebug()<<"GEARS:"<<_gears.keys();

          newGear->load(gearElem,lmf);
          
          // if the gear exists and we don't update (eg: when pasting)
          // we must give it a unique UUID
          if(existingGear && !lmf.testFlag(Drone::UpdateWhenPossible))
          {
            // the gear UUID must be newUUID. It currently has oldUUID because
            // of the previous call to "load". Change this and note it in the rename map
            newGear->setUUID(newUUID);
            renameMap[oldUUID] = newUUID;
          }
          else
          {
            // the gear UUID will be oldUUID. It's already attributed to the gear
            // by the previous call to "load". Last thing, we must correct the _gears registry
            // because newGear has been added to it with it's initial "newUUID"
            _gears.remove(newUUID);
            _gears[newGear->getUUID()]=newGear;
          }
          visitedGears<<newGear->getUUID();
        }

      }

    }
    gearNode = gearNode.nextSibling();
  }
  
  // delete untouched gears
  if(lmf & Drone::DeleteUnvisitedElements)
  {
    QSet<QString> allGears(QSet<QString>::fromList(getGearsUUID()));
    //qDebug()<<"allGears:"<<allGears;
    //qDebug()<<"visitedGears:"<<visitedGears;
    QList<QString> uuidsToDelete(allGears.subtract(visitedGears).toList());
    QList<Gear*> gearsToDelete(getGearsByUUID(uuidsToDelete));
    //qDebug()<<"gears to delete"<<uuidsToDelete;
    foreach(Gear* g, gearsToDelete)
    {
      removeDeepGear(g);
    }
  }
   
  
    
  //load connections    
  QDomNode connectionsNode = XMLHelper::findChildNode(parent, "Connections");

  if (connectionsNode.isNull())
  {
    qDebug() << "Bad DroneSchema : <Connections> tag not found!"  ;
    return false;
  }

  //iteration on all connections
  QDomNode connectionNode = connectionsNode.firstChild();
  Connection connection;
  while (!connectionNode.isNull())
  {
    QDomElement connectionElem = connectionNode.toElement();

    if (!connectionElem.isNull())
    {
      connection.load(connectionElem);
      
      visitedConnections<<connection.toString();

      // if we're remapping uuids, account for that change in connection targets
      if(!(lmf & Drone::UpdateWhenPossible))
        connection.updateWithRenameMapping(renameMap);
      
      //qDebug()<<"after:"<<connection;
      if(!connect(connection))
        qCritical()<<"Could not connect!:"<<connection;
    }

    connectionNode = connectionNode.nextSibling();
  }               


  // delete untouched connections
  if(lmf & Drone::DeleteUnvisitedElements)
  {
    QSet<QString> connectionsToDisconnect(existingConnections.subtract(visitedConnections));
  
    //qDebug()<<"Connections to disconnect: "<<connectionsToDisconnect;

    foreach(QString cstr, connectionsToDisconnect)
    {
      Connection c(cstr);
      Gear * g1(getGearByUUID(c.gearA()));
      Gear * g2(getGearByUUID(c.gearB()));
      if(!g1 || !g2)
        qCritical()<<"Can't unconnect "<<c;
      else
      {
        if(!disconnect(*(g1->getPlug(c.output())),*(g2->getPlug(c.input()))))
          qDebug()<<"!!!Could not Delete connection:"<<c;
      }
      // delete temporaryObject
    }
  }
  return true;
}


/**
 * unlock the schema and perform all scheduled operations
 */
void Schema::unlock()
{
  _locked=false;

  //perform all scheduled operations
  
  //delete
  for (QList<Gear*>::iterator it=_scheduledDeletes.begin(); it!=_scheduledDeletes.end(); ++it)
    removeDeepGear(*it);

  _scheduledDeletes.clear();

  //connect
  for (QList<ScheduledConnection>::iterator it=_scheduledConnections.begin(); it != _scheduledConnections.end(); ++it)
    connect(*((*it)._a), *((*it)._b));
  
  _scheduledConnections.clear();

  //disconnect
  for (QList<ScheduledConnection>::iterator it=_scheduledDisconnections.begin(); it != _scheduledDisconnections.end(); ++it)
    disconnect(*((*it)._a), *((*it)._b));

  _scheduledDisconnections.clear();  
}

Schema *Schema::getParentSchema()
{
  return _parentMetaGear->parentSchema();
}



/*
void Schema::onGearAdded(Gear &gear)
{
	emit gearAdded(*this, gear);
	
  //foward up the event to the parent schema
  Schema *parentSchema = getParentSchema();
  if (parentSchema)
    parentSchema->onGearAdded(gear);
}

void Schema::onGearRemoved(Gear &gear)
{
	emit gearRemoved(*this, gear);
	
  //foward up the event to the parent schema
  Schema *parentSchema = getParentSchema();
  if (parentSchema)
    parentSchema->onGearRemoved(gear);
}
*/

