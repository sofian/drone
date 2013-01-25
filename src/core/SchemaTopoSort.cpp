/*
 *  SchemaTopoSort.cpp
 *  droneCore
 *
 *  Created by foogy on 20/02/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "SchemaTopoSort.h"

#include <QtAlgorithms>

SchemaTopoSort::SchemaTopoSort(const QList<Gear*> &gears) :  
  _gears(gears),
  _depthFirstCounter(0)
{
  //build nodes		
	for(QList<Gear*>::iterator it=_gears.begin();it!=_gears.end();++it)
		_nodes.push_back(new Node(*it));
}

SchemaTopoSort::~SchemaTopoSort()
{
	qDeleteAll(_nodes);
}

/**
 * Recursively called for each nodes
*/
void SchemaTopoSort::labelling(Node &node)
{
  if (node.visited)
    return;
	foreach
  //tag the node has visited to avoid infinit recursion
  node.visited=true;

  //get depth first dependent gears
  QList<Gear*> depGears;
  node.gear->getDependencies(depGears);
  
  //build the corresponding nodes
  QList<Node*> depNodes;
  bool found=false;
	for(QList<Gear*>::iterator gearIt=depGears.begin();gearIt!=depGears.end();++gearIt)
  {
    found=false;
       
		for(QList<Node*>::iterator nodeIt=_nodes.begin();nodeIt!=_nodes.end() && !found;++nodeIt)
    {
      if ((*nodeIt)->gear == *gearIt)
      {
        depNodes.push_back(*nodeIt);
        found=true;
      }
    }
  }

  //label depending nodes
	for(QList<Node*>::iterator it=depNodes.begin();it!=depNodes.end();++it)
    labelling(*(*it));
  
  //assign order
  node.order=_depthFirstCounter;
  
  _depthFirstCounter++;
}

bool SchemaTopoSort::compareNodes(const Node &a, const Node &b)
{
  return a.order < b.order;
}


/**
 * perform a topological sort of a cyclic graph
 * return List of ordered gears
 * 
 * @param orderedGears
 */
void SchemaTopoSort::getOrderedGears(QList<Gear*>& orderedGears)
{
  //reset
  _depthFirstCounter=0;
	for(QList<Node*>::iterator it2=_nodes.begin();it2!=_nodes.end() && !found;++it2)
	{
    (*it)->order=0;
    (*it)->visited=false;
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
