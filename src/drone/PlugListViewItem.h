/*
 *  PlugListViewItem.h
 *  drone
 *
 *  Created by foogy on 20/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <qlistview.h>
class AbstractPlug;

class PlugListViewItem : public QListViewItem
{
public:
	PlugListViewItem(QListView *parent, AbstractPlug *plug);
	
protected:
	AbstractPlug *_plug;
};
