/*
 *  PlugsTableItem.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PlugsTABLEITEM_INCLUDED
#define PlugsTABLEITEM_INCLUDED

#include <qtable.h>

class AbstractPlug;

class PlugsTableItem : public QTableItem
{
public:  
  PlugsTableItem(AbstractPlug *plug, QTable *table, EditType et, const QString &text);
  virtual ~PlugsTableItem();

protected:
  AbstractPlug *_plug;
};
                               
#endif                               
