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

#include <q3table.h>

class AbstractPlug;

class PlugPropertiesTableItem : public Q3TableItem
{
public:  
  PlugPropertiesTableItem(AbstractPlug *plug, Q3Table *table, EditType et, const QString &text);
  virtual ~PlugPropertiesTableItem();

protected:
  AbstractPlug *_plug;
};
                               
#endif                               
