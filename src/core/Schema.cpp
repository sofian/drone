#include "Schema.h"
#include "Gear.h"
#include "MetaGear.h"
#include "GearMaker.h"

#include <qdom.h>
#include <qfile.h>
#include <qtextstream.h>


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

Gear* Schema::getDeepGearByName(std::string name) const
{
  std::list<Gear*> allgears = getDeepGears();

  for (std::list<Gear*>::const_iterator it = allgears.begin();it!=allgears.end();++it)
  {
    if ((*it)->name() == name)
      return(*it);
  }

  return NULL;
}

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
_parentMetaGear(parentMetaGear)
{
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

  setSynched();

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

std::string Schema::getNewGearName(std::string prefix)
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

void Schema::setSynched()
{
  _needSynch=false;
  std::list<Schema*> subschemas = getSubSchemas();
  for(std::list<Schema*>::iterator it = subschemas.begin(); it!=subschemas.end();++it)
    (*it)->setSynched();
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

bool Schema::removeDeepGear(Gear* gear)
{
  ASSERT_ERROR(gear!=NULL);
  
  // if the gear to be removed is at the current schema level, remove it here
  if(find(_gears.begin(),_gears.end(),gear) != _gears.end())
  {   
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

Gear* Schema::addMetaGear(Engine * engine, std::string geartype, std::string name)
{
  MetaGear * testgear = new MetaGear(engine, geartype, name);
  testgear->internalInit();
  _gears.push_back(testgear);

  return testgear;
}


Gear* Schema::addGear(Engine * engine, std::string geartype, std::string name)
{
  Gear *gear = GearMaker::makeGear(engine, geartype, name);

  if (gear==NULL)
    std::cout << "Schema addGear: " << geartype << " unknown" << std::endl;
  else
  {
    gear->internalInit();
    _gears.push_back(gear);
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
        connections.push_back( new Connection((*itGear)->name(), (*itOutput)->name(), 
                                              (*itConnectedPlug)->parent()->name(), 
                                              (*itConnectedPlug)->name()));
      }            
    }
  }
}


void Schema::connectPlugs(Schema::Connection &connection)
{
   Gear *gearA;                                                                             
   Gear *gearB;                                                                             
   AbstractPlug *input;                                                                     
   AbstractPlug *output;                                                                    
                                                                                            
   if ( (gearA=getDeepGearByName(connection.gearA())) == NULL)                                        
   {                                                                                        
     std::cout << "connectPlugs fail: " + connection.gearA() + " not found!" << std::endl;  
     return;                                                                                
   }                                                                                        
                                                                                            
                                                                                            
   if ( (gearB=getDeepGearByName(connection.gearB())) == NULL)                                        
   {                                                                                        
     std::cout << "connectPlugs fail: " + connection.gearB() + " not found!" << std::endl;  
     return;                                                                                
   }                                                                                        
                                                                                            
   if ( (output=gearA->getOutput(connection.output())) == NULL)                             
   {                                                                                        
     std::cout << "connectPlugs fail: " + connection.output() + " not found!" << std::endl; 
     return;                                                                                
   }                                                                                        
                                                                                            
   if ( (input=gearB->getInput(connection.input())) == NULL)                                
   {                                                                                        
     std::cout << "connectPlugs fail: " + connection.input() + " not found!" << std::endl;  
     return;                                                                                
   }                                                                                        
                                                                                            
   if (!output->connect(input))                                                             
   {                                                                                        
     std::cout << "connectPlugs fail!" << std::endl;                                        
   }                                                                                        

}

void Schema::save(std::string filename)
{
  QDomDocument doc("DroneSchema");
  QDomElement rootElem = doc.createElement("Schema");
  doc.appendChild(rootElem);


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

  //save to file  
       
  QFile file(filename.c_str());
  if (file.open(IO_WriteOnly))
  {
    QTextStream stream(&file);
    doc.save(stream,4);
    file.close();
  }
  else
    std::cout << "file io error, cannot save!" << std::endl;

}

void Schema::load(Engine * engine, std::string filename)
{        
  QDomDocument doc("DroneSchema");

  QFile file(filename.c_str());

  if (!file.open(IO_ReadOnly))
  {
    std::cout << "Fail to open file " << filename << std::endl;
    return;
  }

  clear();

  QString errMsg;
  int errLine;
  int errColumn;
  if (!doc.setContent(&file, true, &errMsg, &errLine, &errColumn))
  {
    std::cout << "parsing error in " << filename << std::endl;
    std::cout << errMsg.ascii() << std::endl;
    std::cout << "Line: " <<  errLine << std::endl;
    std::cout << "Col: " <<  errColumn << std::endl;
    file.close();
    return;
  }

  file.close();

  //load gears
  QDomNodeList gearsNode = doc.elementsByTagName("Gears");

  //be sure we have exactly one Gears tag
  if (gearsNode.count()==1)
  {
    //iteration on all gears
    QDomNode gearNode = gearsNode.item(0).firstChild();
    Gear *pgear=NULL;
    while (!gearNode.isNull())
    {
      QDomElement gearElem = gearNode.toElement();
      if (!gearElem.isNull())
      {
        pgear = addGear(engine, gearElem.attribute("Type","").ascii(), gearElem.attribute("Name","").ascii());
        if (pgear!=NULL)
        {
          pgear->internalLoad(gearElem);                                
        }

      }
      gearNode = gearNode.nextSibling();
    }                
  } else
    std::cout << "Bad DroneSchema : problem with <Gears>" << std::endl;


  //load connections    
  QDomNodeList connectionsNode = doc.elementsByTagName("Connections");

  //be sure we have exactly one Connections tag
  if (connectionsNode.count()==1)
  {
    //iteration on all connections
    QDomNode connectionNode = connectionsNode.item(0).firstChild();
    Connection connection;
    while (!connectionNode.isNull())
    {
      QDomElement connectionElem = connectionNode.toElement();

      if (!connectionElem.isNull())
      {
        connection.load(connectionElem);            
        connectPlugs(connection);
      }

      connectionNode = connectionNode.nextSibling();
    }               
  } else
    std::cout << "Bad DroneSchema : problem with <Connections>" << std::endl;

}
