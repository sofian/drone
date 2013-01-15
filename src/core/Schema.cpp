#include "Schema.h"
#include "Gear.h"
#include "MetaGear.h"
#include "GearMaker.h"
//#include "SchemaTopoSort.h"
#include "Connection.h"


#include <QtXml>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include "XMLHelper.h"

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

Gear* Schema::getGearByName(QString name) const
{
  return _gears.value(name, NULL);
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

/**
 * Mangles a unique gear name by adding a number after the name in this schema only.
 * 
 * @param originalName
 * 
 * @return 
 */
QString Schema::mangleUniqueGearName(QString originalName)
{
  if (!_gears.contains(originalName))
		return originalName;

	int i=1;
	QString mangledName = originalName + QString::number(i);
	
	while(_gears.contains(mangledName))
	{
		i++;
		mangledName = originalName + QString::number(i);
	}
	
	return mangledName;
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
  if (_gears.contains(gear->name()))
  {   
    emit gearRemoved(*this,*gear);
    
    _gears.remove(gear->name());
            
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
	
  gear.name(mangleUniqueGearName(newName));
}

bool Schema::addGear(Gear& gear)
{		
	//make unique naming
	gear.name(mangleUniqueGearName(gear.name()));
	
	//add to the schema
	_gears.insert(gear.name(), &gear);
	gear.parentSchema(*this);

	emit gearAdded(*this, gear);
  return true;
}

//not deep
void Schema::getAllConnections(QList<Connection*> &connections)
{
  QList<AbstractPlug*> outputs;
  QList<AbstractPlug*> connectedPlugs;

  connections.clear();

	foreach(Gear *gear, _gears)  
	{
    gear->getOutputs(outputs);
    for (QList<AbstractPlug*>::Iterator itOutput = outputs.begin(); itOutput != outputs.end(); ++itOutput)
    {
      (*itOutput)->connectedPlugs(connectedPlugs);

      for (QList<AbstractPlug*>::iterator itConnectedPlug = connectedPlugs.begin(); itConnectedPlug != connectedPlugs.end(); ++itConnectedPlug)
      {
        
        //in the case of an exposed plug, 
        //just be sure that the destination gear is really in our schema
        if ( (!(*itOutput)->exposed()) || 
             ((*itOutput)->exposed() && (getGearByName((*itConnectedPlug)->parent()->name()))) )
          connections.push_back( new Connection(gear->name(), (*itOutput)->name(), 
                                                (*itConnectedPlug)->parent()->name(), 
                                                (*itConnectedPlug)->name()));
      }            
    }
  }
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
    return plugA.connect(&plugB);  
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
void Schema::disconnect(AbstractPlug &plugA, AbstractPlug &plugB)
{
  
  if (_locked)
  {
    _scheduledDisconnections.push_back(ScheduledConnection(plugA, plugB));
  }
  else
    plugA.disconnect(&plugB);
}

void Schema::disconnectAll(AbstractPlug &plug)
{
  
  plug.disconnectAll();
}

bool Schema::connect(Connection &connection)
{
   Gear *gearA;                                                                             
   Gear *gearB;                                                                             
   AbstractPlug *input;                                                                     
   AbstractPlug *output;                                                                    
                                                                                            
   if ( (gearA=getGearByName(connection.gearA())) == NULL)                                        
   {                                                                                        
     qCritical() << "connectPlugs fail: " + connection.gearA() + " not found!";  
     return false;                                                                                
   }                                                                                        
                                                                                                                                                                                        
   if ( (gearB=getGearByName(connection.gearB())) == NULL)                                        
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
                                                                                            
   return output->connect(input);
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
//    if( onlySelected && ( ggui==NULL || !( ggui->isSelected() ) ))
//      continue;
    std::cerr<<"About to save!"<<std::endl;
    gear->save(doc, gearsElem);            
  }
	


  //save all connections
  QDomElement connectionsElem = doc.createElement("Connections");
  rootElem.appendChild(connectionsElem);

  QList<Connection*> connections;
  getAllConnections(connections);

  for (QList<Connection*>::iterator it = connections.begin(); it != connections.end(); ++it)
  {
    Gear * gA = getGearByName((*it)->gearA());
    Gear * gB = getGearByName((*it)->gearB());
/*
    if(onlySelected 
       && (ggA == NULL || !ggA->isSelected() || ggB == NULL || !ggB->isSelected()))
      continue;
*/       
    (*it)->save(doc, connectionsElem);
    delete (*it);//free
  }
    
  return true;
}

bool Schema::load(QDomElement& parent, bool onlySelected)
{    
  std::vector<Gear*> addedGears;
  QDomNode gearsNode = XMLHelper::findChildNode(parent, "Gears");
  // when pasting, gears have to be renamed
  std::map<QString,QString> renameMap;
  if (gearsNode.isNull())
  {
    std::cout << "Bad DroneSchema : <Gears> tag not found!" << std::endl;
    return false;
  }

  QDomNode gearNode = gearsNode.firstChild();
  Gear *pgear=NULL;
  while (!gearNode.isNull())
  {
    QDomElement gearElem = gearNode.toElement();
    if (!gearElem.isNull())
    {
			pgear = GearMaker::instance()->makeGear(gearElem.attribute("Type",""));
			/*
      if (type == MetaGear::TYPE)        
        //we default the name to metagear, but the name will be overwrited in the load of the metagear itself
        pgear = addMetaGear("MetaGear", gearElem.attribute("Name",""));          
      else                   
        pgear = addGear(type, gearElem.attribute("Name",""));
      */
      if (pgear!=NULL)
      {
				addGear(*pgear);
        pgear->load(gearElem);                                
      }              
                  
      addedGears.push_back(pgear);

    }
    gearNode = gearNode.nextSibling();
  }                
  /*
  if(pasting)
    for(unsigned int i=0;i<addedGears.size();++i)
    {
      QString newname = getUniqueGearName(addedGears[i]->type());
      renameMap[addedGears[i]->name()] = getUniqueGearName(addedGears[i]->type());

      addedGears[i]->name(renameMap[addedGears[i]->name()]);
    }
  */  
  //load connections    
  QDomNode connectionsNode = XMLHelper::findChildNode(parent, "Connections");

  if (connectionsNode.isNull())
  {
    std::cout << "Bad DroneSchema : <Connections> tag not found!" << std::endl;
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
	/*
      if(pasting)
        connection.updateWithRenameMapping(renameMap);
  */
			    connect(connection);
    }

    connectionNode = connectionNode.nextSibling();
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

