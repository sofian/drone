#include "Schema.h"
#include "Gear.h"
#include "MetaGear.h"
#include "GearMaker.h"

#include <qdom.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include "XMLHelper.h"

const std::string Schema::XML_TAGNAME = "Schema";

void Schema::Connection::save(QDomDocument &doc, QDomElement &parent)
{
  QDomElement connectionElem = doc.createElement("Connection");
  parent.appendChild(connectionElem);

  QDomAttr gearA = doc.createAttribute("GearA");
  gearA.setValue(_gearA.c_str());
  connectionElem.setAttributeNode(gearA);

  QDomAttr input = doc.createAttribute("Input");
  input.setValue(_input.c_str());
  connectionElem.setAttributeNode(input);

  QDomAttr gearB = doc.createAttribute("GearB");
  gearB.setValue(_gearB.c_str());
  connectionElem.setAttributeNode(gearB);

  QDomAttr output = doc.createAttribute("Output");
  output.setValue(_output.c_str());
  connectionElem.setAttributeNode(output);

}

void Schema::Connection::load(QDomElement &connectionElem)
{
  _gearA = connectionElem.attribute("GearA","").ascii();
  _input = connectionElem.attribute("Input","").ascii();
  _gearB = connectionElem.attribute("GearB","").ascii();
  _output = connectionElem.attribute("Output","").ascii();
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

/* Gear* Schema::getDeepGearByName(std::string name) const                                */
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

Gear* Schema::getGearByName(std::string name) const
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

  std::vector<Gear*> readyGears;
  std::list<Gear*> orderedGears;
  std::list<Gear*> internalSchemaOrderedGears;

  _lastDeepOrderedReadyGears.clear();

  for (std::list<Gear*>::iterator it=_gears.begin();it!=_gears.end();++it)
  {
    if ((*it)->ready())
      readyGears.push_back(*it);
  }

  GearGraphManip ggm(readyGears);
  ggm.getOrderedGears(orderedGears);

  for(std::list<Gear*>::iterator it=orderedGears.begin();it!=orderedGears.end();++it)
  {
    if ((*it)->getInternalSchema()!=NULL)
    {
      internalSchemaOrderedGears.clear();
      internalSchemaOrderedGears = (*it)->getInternalSchema()->getDeepOrderedReadyGears();
      _lastDeepOrderedReadyGears.insert(_lastDeepOrderedReadyGears.end(), internalSchemaOrderedGears.begin(), internalSchemaOrderedGears.end());  
    }
    else
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
std::string Schema::getUniqueGearName(std::string prefix)
{
  int i=1;
  std::string tmp;
  bool ok=false;
  char buf[10];
  while (!ok && i<200)
  {
    ok=true;
    sprintf(buf,"%i",i);
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
    _parentMetaGear->onGearRemoved(gear);
    delete gear;
    _gears.remove(gear);    
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
  gear->internalInit();
}

MetaGear* Schema::newMetaGear()
{
  std::string name="MetaGear";
  return addMetaGear(name, getUniqueGearName(name));
}

MetaGear* Schema::addMetaGear(std::string filename)
{
  QFileInfo fileInfo(filename);
  std::string name = fileInfo.baseName();

  MetaGear *metaGear = new MetaGear(this, name, getUniqueGearName(name));

  initGear(metaGear);

  if (!metaGear->load(filename))
  {
    delete metaGear;
    return NULL;
  }
  
  _gears.push_back(metaGear);
  _parentMetaGear->onGearAdded(metaGear);

  return metaGear;
}

void Schema::renameMetaGear(MetaGear* metaGear, std::string newName)
{
  if (!metaGear)
    return;
  
  metaGear->name(getUniqueGearName(newName));
}

MetaGear* Schema::addMetaGear(std::string name, std::string uniqueName)
{
  MetaGear *metaGear = new MetaGear(this, name, uniqueName);
  initGear(metaGear);
  _gears.push_back(metaGear);
  
  _parentMetaGear->onGearAdded(metaGear);

  return metaGear;
}

Gear* Schema::addGear(std::string geartype)
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
Gear* Schema::addGear(std::string geartype, std::string uniqueName)
{
  
  Gear *gear = GearMaker::makeGear(this, geartype, uniqueName);

  if (gear==NULL)
    std::cout << "Schema addGear: " << geartype << " unknown" << std::endl;
  else
  {
    initGear(gear);    
    //gear->internalInit();
    _gears.push_back(gear);
    _parentMetaGear->onGearAdded(gear);
  }

  return gear;
}

//not deep
void Schema::getAllConnections(std::list<Connection*> &connections)
{
  std::list<AbstractPlug*> outputs;
  std::list<AbstractPlug*> connectedPlugs;

  connections.clear();

  for (std::list<Gear*>::iterator itGear = _gears.begin(); itGear != _gears.end(); ++itGear)
  {
    (*itGear)->getOutputs(outputs);
    for (std::list<AbstractPlug*>::iterator itOutput = outputs.begin(); itOutput != outputs.end(); ++itOutput)
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
     std::cout << "connectPlugs fail: " + connection.gearA() + " not found!" << std::endl;  
     return false;                                                                                
   }                                                                                        
                                                                                                                                                                                        
   if ( (gearB=getGearByName(connection.gearB())) == NULL)                                        
   {                                                                                        
     std::cout << "connectPlugs fail: " + connection.gearB() + " not found!" << std::endl;  
     return false;                                                                                
   }                                                                                        
                                                                                            
   if ( (output=gearA->getOutput(connection.output())) == NULL)                             
   {                                                                                        
     std::cout << "connectPlugs fail: " + connection.output() + " not found!" << std::endl; 
     return false;                                                                                
   }                                                                                        
                                                                                            
   if ( (input=gearB->getInput(connection.input())) == NULL)                                
   {                                                                                        
     std::cout << "connectPlugs fail: " + connection.input() + " not found!" << std::endl;  
     return false;                                                                                
   }                                                                                        
                                                                                            
   return output->connect(input);
}

bool Schema::save(QDomDocument& doc, QDomElement &parent)
{
  QDomElement rootElem = doc.createElement(XML_TAGNAME);
  parent.appendChild(rootElem);


  //save all gears
  QDomElement gearsElem = doc.createElement("Gears");
  rootElem.appendChild(gearsElem);

  for (std::list<Gear*>::iterator it=_gears.begin();it!=_gears.end();++it)
  {
    (*it)->internalSave(doc, gearsElem);            
  }


  //save all connections
  QDomElement connectionsElem = doc.createElement("Connections");
  rootElem.appendChild(connectionsElem);

  std::list<Connection*> connections;
  getAllConnections(connections);

  for (std::list<Connection*>::iterator it = connections.begin(); it != connections.end(); ++it)
  {
    (*it)->save(doc, connectionsElem);
    delete (*it);//free
  }
    
  return true;
}

bool Schema::load(QDomElement& parent)
{          
  QDomNode gearsNode = XMLHelper::findChildNode(parent, "Gears");

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
      std::string type = gearElem.attribute("Type","").ascii();
      
      if (type == MetaGear::TYPE)        
        //we default the name to metagear, but the name will be overwrited in the load of the metagear itself
        pgear = addMetaGear("MetaGear", gearElem.attribute("Name","").ascii());          
      else                   
        pgear = addGear(type, gearElem.attribute("Name","").ascii());
      
      if (pgear!=NULL)
      {
        pgear->internalLoad(gearElem);                                
      }                                

    }
    gearNode = gearNode.nextSibling();
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

