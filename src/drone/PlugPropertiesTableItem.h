/*
 *  PlugPropertiesTableItem.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PLUGPROPERTIESTABLEITEM_INCLUDED
#define PLUGPROPERTIESTABLEITEM_INCLUDED

#include <qtable.h>

class AbstractPlug;

class PlugPropertiesTableItem : public QTableItem
{
public:  
  PlugPropertiesTableItem(AbstractPlug *plug, QTable *table, EditType et);
  virtual ~PlugPropertiesTableItem();

protected:
  AbstractPlug *_plug;
};
                               
#endif                               
