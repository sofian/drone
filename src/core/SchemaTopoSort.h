/*
 *  SchemaTopoSort.h
 *  droneCore
 *
 *  Created by foogy on 20/02/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SCHEMATOPOSORT_INCLUDED
#define SCHEMATOPOSORT_INCLUDED

#include <QList>

class SchemaTopoSort
{
public :

	SchemaTopoSort(QList<Gear*> &gears);    
	void getOrderedGears(QList<Gear*>& orderedGears);
	void labelling(Node &node);

protected:
	static bool compareNodes(const Node &a, const Node &b);

	QList<Node*> _nodes;
	QList<Gear*> _gears;
	int _depthFirstCounter;
};

#endif