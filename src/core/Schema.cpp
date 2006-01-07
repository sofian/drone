#include "Schema.h"
#include "Gear.h"
#include "MetaGear.h"
#include "GearMaker.h"
#include "ISchemaEventListener.h"

#include <QtXml>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include "XMLHelper.h"

const QString Schema::XML_TAGNAME = "Schema";

void Schema::Connection::save(QDomDocument &doc, QDomElement &parent)
{
  QDomElement connectionElem = doc.createElement("Connection");
  parent.appendChild(connectionElem);

  QDomAttr gearA = doc.createAttribute("GearA");
  gearA.setValue(_gearA);
  connectionElem.setAttributeNode(gearA);

  QDomAttr input = doc.createAttribute("Input");
  input.setValue(_input);
  connectionElem.setAttributeNode(input);

  QDomAttr gearB = doc.createAttribute("GearB");
  gearB.setValue(_gearB);
  connectionElem.setAttributeNode(gearB);

  QDomAttr output = doc.createAttribute("Output");
  output.setValue(_output);
  connectionElem.setAttributeNode(output);

}

void Schema::Connection::load(QDomElement &connectionElem)
{
  _gearA = connectionElem.attribute("GearA","");
  _input = connectionElem.attribute("Input","");
  _gearB = connectionElem.attribute("GearB","");
  _output = connectionElem.attribute("Output","");
}

void Schema::Connection::updateWithRenameMapping(std::map<QString,QString> map)
{
  std::map<QString,QString>::iterator renA(map.find(_gearA));
  std::map<QString,QString>::iterator renB(map.find(_gearB));
  if(renA!=map.end())
    _gearA = map[_gearA];
  if(renB!=map.end())
    _gearB = map[_gearB];
}

Schema::GearGraphManip::GearGraphManip(std::vector<Gear*> &gears) :  
  _gears(gears),
  _depthFirstCounter(0)
{
  //build nodes
  for (unsigned int i=0;i<_gears.size();++i)  
    _nodes.push_back(Node(_gears[i]));    
}

void Schema::GearGraphManip::labelling(Node &node)
{
  if (node.visited)
    return;

  //tag the node has visited to avoid infinit recursion
  node.visited=true;

  //get depth fist dependent gears
  std::vector<Gear*> depGears;
  node.gear->getDependencies(depGears);
  
  //build the corresponding nodes vector
  std::vector<Node*> depNodes;
  bool found=false;
  for (unsigned int i=0;i<depGears.size();++i)
  {
    found=false;
       
    for (unsigned int j=0;j<_nodes.size() && !found;++j)
    {
      if (_nodes[j].gear == depGears[i])
      {
        depNodes.push_back(&_nodes[j]);
        found=true;
      }
    }
  }

  //label depending nodes
  for (unsigned int i=0;i<depNodes.size();++i)
    labelling(*depNodes[i]);
  
  //assign order
  node.order=_depthFirstCounter;
  
  //inc global counter
  _depthFirstCounter++;
}

bool Schema::GearGraphManip::compareNodes(const Node &a, const Node &b)
{
  return a.order < b.order;
}


/**
 * perform a topological sort of a cyclic graph
 * return vector of ordered gears
 * 
 * @param orderedGears
 */
void Schema::GearGraphManip::getOrderedGears(std::list<Gear*>& orderedGears)
{
  //reset
  _depthFirstCounter=0;
  for (unsigned int i=0;i<_nodes.size();++i)
  {
    _nodes[i].order=0;
    _nodes[i].visited=false;
  }

  for (unsigned int i=0;i<_nodes.size();++i)
  {
    if (!_nodes[i].visited)
      labelling(_nodes[i]);  
  }
  
  //sort according to order
  std::sort(_nodes.begin(), _nodes.end(), compareNodes);
  
  //fill the ordered gears vector now
  orderedGears.clear();
  for (unsigned int i=0;i<_nodes.size();++i)
    orderedGears.push_back(_nodes[i].gear);
}

/* Gear* Schema::getDeepGearByName(QString name) const                                */
/* {                                                                                      */
/*   std::list<Gear*> allgears = getDeepGears();                                          */
/*                                                                                        */
/*   for (std::list<Gear*>::const_iterator it = allgears.begin();it!=allgears.end();++it) */
/*   {                                                                                    */
/*     if ((*it)->name() == name)                                                         */
/*       return(*it);                                                                     */
/*   }                                                                                    */
/*                                                                                        */
/*   return NULL;                                                                         */
/* }                                                                                      */

Gear* Schema::getGearByName(QString name) const
{
  for (std::list<Gear*>::const_iterator it = _gears.begin();it!=_gears.end();++it)
  {
    if ((*it)->name() == name)
      return(*it);
  }

  return NULL;
}

Schema::Schema(MetaGear * parentMetaGear) :
_needSynch(true),  
_parentMetaGear(parentMetaGear),
_locked(false)
{
  ASSERT_ERROR_MESSAGE(_parentMetaGear!=0, "Schema with a null parent metagear");
}

Schema::~Schema()
{
  clear();
}

std::list<Gear*> Schema::getDeepOrderedReadyGears()
{
  if(!needSynch())
    return _lastDeepOrderedReadyGears;

  std::list<Gear*> deepOrderedGears;
  std::list<Gear*> orderedGears;
  std::list<Gear*> internalSchemaOrderedGears;

  _lastDeepOrderedReadyGears.clear();

	std::vector<Gear*> gearList(_gears.begin(), _gears.end()); 
  GearGraphManip ggm(gearList);
	
  ggm.getOrderedGears(orderedGears);

  for(std::list<Gear*>::iterator it=orderedGears.begin();it!=orderedGears.end();++it)
  {
    if ((*it)->getInternalSchema()!=NULL)
    {
      internalSchemaOrderedGears.clear();
      internalSchemaOrderedGears = (*it)->getInternalSchema()->getDeepOrderedReadyGears();
      deepOrderedGears.insert(deepOrderedGears.end(), internalSchemaOrderedGears.begin(), internalSchemaOrderedGears.end());  
    }
    else
      deepOrderedGears.push_back(*it);
  }


	//filter on ready gears only
	//first pass, evaluateReady for all gears
	for(std::list<Gear*>::iterator it=deepOrderedGears.begin();it!=deepOrderedGears.end();++it)
	{
		(*it)->evaluateReady();
	}
		
	//finally, filter
	for(std::list<Gear*>::iterator it=deepOrderedGears.begin();it!=deepOrderedGears.end();++it)
	{
		if ((*it)->ready())
			_lastDeepOrderedReadyGears.push_back(*it);
	}
  
	_needSynch=false;

  return _lastDeepOrderedReadyGears;
}

std::list<Gear*> Schema::getDeepGears() const
{
  std::list<Gear*> deepGears;
  std::list<Gear*> internalSchemaGears;

  for(std::list<Gear*>::const_iterator it=_gears.begin();it!=_gears.end();++it)
  {
    if ((*it)->getInternalSchema()!=NULL)
    {
      internalSchemaGears.clear();
      internalSchemaGears = (*it)->getInternalSchema()->getDeepGears();
      deepGears.insert(deepGears.end(), internalSchemaGears.begin(), internalSchemaGears.end());  
    }
    else
      deepGears.push_back(*it);
  }

  return deepGears;
}

/**
 * get a unique name for a gear in this schema only
 * 
 * @param prefix
 * 
 * @return 
 */
QString Schema::getUniqueGearName(QString prefix)
{
  int i=1;
  QString tmp;
  bool ok=false;
  char buf[10];
  // in drone specs : drone supports up to 123456 gears 
  while (!ok && i<123456)
  {
    ok=true;
    sprintf(buf,".%i",i);
    tmp=prefix + buf;
    for (std::list<Gear*>::iterator it=_gears.begin();it!=_gears.end();++it)
      if ((*it)->name() == tmp)
      {
        ok=false;
        break;
      }
    i++;
  }
  return tmp; 
}

bool Schema::needSynch()
{
  if(_needSynch)
    return true;
  
  std::list<Schema*> subschemas = getSubSchemas();
  for(std::list<Schema*>::iterator it = subschemas.begin(); it!=subschemas.end();++it)
    if((*it)->needSynch())
      return true;
  
  return false;
}

void Schema::synch()
{
  _needSynch=false;
  std::list<Schema*> subschemas = getSubSchemas();
  for(std::list<Schema*>::iterator it = subschemas.begin(); it!=subschemas.end();++it)
    (*it)->synch();
}


void Schema::clear()
{
  for (std::list<Gear*>::iterator it = _gears.begin(); it != _gears.end(); ++it)
    delete *it;

  _gears.clear();
}

std::list<Schema*> Schema::getSubSchemas()
{
  std::list<Schema*> sub;
  for (std::list<Gear*>::iterator it = _gears.begin(); it != _gears.end(); ++it)
    if((*it)->getInternalSchema()!=NULL)
      sub.push_back((*it)->getInternalSchema());
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

bool Schema::removeDeepGear(Gear* gear)
{
  ASSERT_ERROR(gear!=NULL);
  
  if (_locked)
  {
    _scheduledDeletes.push_back(gear);
    return true;
  }

  // if the gear to be removed is at the current schema level, remove it here
  if(find(_gears.begin(),_gears.end(),gear) != _gears.end())
  {   
    onGearRemoved(gear);
    
    _gears.remove(gear);
            
    delete gear;
    
    return true;
  }

  // otherwise, search recursively in subschemas
  std::list<Schema*> subSchemas = getSubSchemas();
  for(std::list<Schema*>::iterator it = subSchemas.begin();it!=subSchemas.end();++it)
    if((*it)->removeDeepGear(gear))
      return true;
  return false;
}

void Schema::initGear(Gear * gear) const
{
  gear->init();
}

MetaGear* Schema::newMetaGear()
{
  QString name="MetaGear";
  return addMetaGear(name, getUniqueGearName(name));
}

MetaGear* Schema::addMetaGear(QString filename)
{
  QFileInfo fileInfo(filename);
  QString name = fileInfo.baseName();

  MetaGear *metaGear = new MetaGear(this, name, getUniqueGearName(name));

  initGear(metaGear);

  if (!metaGear->load(filename))
  {
    delete metaGear;
    return NULL;
  }
  
  _gears.push_back(metaGear);
  onGearAdded(metaGear);

  return metaGear;
}

void Schema::renameGear(Gear* gear, QString newName)
{
  if (!gear)
    return;
  
  gear->name(getUniqueGearName(newName));
}

MetaGear* Schema::addMetaGear(QString name, QString uniqueName)
{
  MetaGear *metaGear = new MetaGear(this, name, uniqueName);
  initGear(metaGear);
  
  _gears.push_back(metaGear);
  
  onGearAdded(metaGear);

  return metaGear;
}                         

Gear* Schema::addGear(QString geartype)
{
  return addGear(geartype, getUniqueGearName(geartype));
}


/**
 * called when loading because we already know the uniqueName
 * 
 * @param geartype
 * @param uniqueName
 * 
 * @return 
 */
Gear* Schema::addGear(QString geartype, QString uniqueName)
{
  
  Gear *gear = GearMaker::makeGear(this, geartype, uniqueName);

  if (gear==NULL)
    qCritical() << "Schema addGear: " << geartype << " unknown";
  else
  {
    initGear(gear);    
        
    _gears.push_back(gear);
    onGearAdded(gear);    
  }

  return gear;
}

//not deep
void Schema::getAllConnections(std::list<Connection*> &connections)
{
  QList<AbstractPlug*> outputs;
  std::list<AbstractPlug*> connectedPlugs;

  connections.clear();

  for (std::list<Gear*>::iterator itGear = _gears.begin(); itGear != _gears.end(); ++itGear)
  {
    (*itGear)->getOutputs(outputs);
    for (QList<AbstractPlug*>::Iterator itOutput = outputs.begin(); itOutput != outputs.end(); ++itOutput)
    {
      (*itOutput)->connectedPlugs(connectedPlugs);

      for (std::list<AbstractPlug*>::iterator itConnectedPlug = connectedPlugs.begin(); itConnectedPlug != connectedPlugs.end(); ++itConnectedPlug)
      {
        
        //in the case of an exposed plug, 
        //just be sure that the destination gear is really in our schema
        if ( (!(*itOutput)->exposed()) || 
             ((*itOutput)->exposed() && (getGearByName((*itConnectedPlug)->parent()->name()))) )
          connections.push_back( new Connection((*itGear)->name(), (*itOutput)->name(), 
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
bool Schema::connect(AbstractPlug *plugA, AbstractPlug *plugB)
{
  if (!plugA || !plugB)
    return false;
  
  if (_locked)
  {
    _scheduledConnections.push_back(ScheduledConnection(plugA, plugB));
    return true;
  }
  else
    return plugA->connect(plugB);  
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
void Schema::disconnect(AbstractPlug *plugA, AbstractPlug *plugB)
{
  if (!plugA || !plugB)
    return;
  
  if (_locked)
  {
    _scheduledDisconnections.push_back(ScheduledConnection(plugA, plugB));
  }
  else
    plugA->disconnect(plugB);
}

void Schema::disconnectAll(AbstractPlug *plug)
{
  if (!plug)
    return;
  
  plug->disconnectAll();
}

bool Schema::connect(Schema::Connection &connection)
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

  for (std::list<Gear*>::iterator it=_gears.begin();it!=_gears.end();++it)
  {
//    if( onlySelected && ( ggui==NULL || !( ggui->isSelected() ) ))
//      continue;
    std::cerr<<"About to save!"<<std::endl;
    (*it)->save(doc, gearsElem);            
  }


  //save all connections
  QDomElement connectionsElem = doc.createElement("Connections");
  rootElem.appendChild(connectionsElem);

  std::list<Connection*> connections;
  getAllConnections(connections);

  for (std::list<Connection*>::iterator it = connections.begin(); it != connections.end(); ++it)
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

bool Schema::load(QDomElement& parent, bool pasting)
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
      QString type = gearElem.attribute("Type","");
      
      if (type == MetaGear::TYPE)        
        //we default the name to metagear, but the name will be overwrited in the load of the metagear itself
        pgear = addMetaGear("MetaGear", gearElem.attribute("Name",""));          
      else                   
        pgear = addGear(type, gearElem.attribute("Name",""));
      
      if (pgear!=NULL)
      {
        pgear->load(gearElem);                                
      }              
                  
      addedGears.push_back(pgear);

    }
    gearNode = gearNode.nextSibling();
  }                
  
  if(pasting)
    for(unsigned int i=0;i<addedGears.size();++i)
    {
      QString newname = getUniqueGearName(addedGears[i]->type());
      renameMap[addedGears[i]->name()] = getUniqueGearName(addedGears[i]->type());

      addedGears[i]->name(renameMap[addedGears[i]->name()]);
    }
    
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
      if(pasting)
        connection.updateWithRenameMapping(renameMap);
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
  for (std::vector<Gear*>::iterator it=_scheduledDeletes.begin(); it!=_scheduledDeletes.end(); ++it)
    removeDeepGear(*it);

  _scheduledDeletes.clear();

  //connect
  for (std::vector<ScheduledConnection>::iterator it=_scheduledConnections.begin(); it != _scheduledConnections.end(); ++it)
    (*it)._a->connect((*it)._b);
  
  _scheduledConnections.clear();

  //disconnect
  for (std::vector<ScheduledConnection>::iterator it=_scheduledDisconnections.begin(); it != _scheduledDisconnections.end(); ++it)
    (*it)._a->disconnect((*it)._b);

  _scheduledDisconnections.clear();  
}

void Schema::onGearAdded(Gear *gear)
{
  for (std::list<ISchemaEventListener*>::iterator it=_schemaEventListeners.begin();it!=_schemaEventListeners.end();++it)
  {
    (*it)->onGearAdded(this, gear);
  }

  //foward up the event to the parent schema
  Schema *parentSchema = getParentSchema();
  if (parentSchema)
    parentSchema->onGearAdded(gear);
}

void Schema::onGearRemoved(Gear *gear)
{
  for (std::list<ISchemaEventListener*>::iterator it=_schemaEventListeners.begin();it!=_schemaEventListeners.end();++it)
  {
    (*it)->onGearRemoved(this, gear);
  }
  
  //foward up the event to the parent schema
  Schema *parentSchema = getParentSchema();
  if (parentSchema)
    parentSchema->onGearRemoved(gear);
  
}

void Schema::addSchemaEventListener(ISchemaEventListener *schemaEventListener)
{
  if (!schemaEventListener)
    return;

  _schemaEventListeners.push_back(schemaEventListener);
}

void Schema::removeSchemaEventListener(ISchemaEventListener *schemaEventListener)
{
  if (!schemaEventListener)
    return;

  _schemaEventListeners.remove(schemaEventListener);
}

Schema *Schema::getParentSchema()
{
  return _parentMetaGear->parentSchema();
}
