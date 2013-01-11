/*
 *  Node.h
 *  droneCore
 *
 *  Created by foogy on 19/03/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
class Gear;
#include <QList>

class Node
{
public:
	Node() :
		order(0),
		visited(false){}

	void getDependencies(QList<Node*> & dependencies);
	int order;
	bool visited;
};
