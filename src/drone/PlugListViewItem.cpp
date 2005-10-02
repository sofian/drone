/*
 *  PlugListViewItem.cpp
 *  drone
 *
 *  Created by foogy on 20/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#include <qpainter.h>

#include "PlugListViewItem.h"
#include "AbstractPlug.h"


PlugListViewItem::PlugListViewItem(QListView *parent, AbstractPlug *plug) :
	QListViewItem(parent),
	_plug(plug)
{
		setText(1, plug->name());
}

