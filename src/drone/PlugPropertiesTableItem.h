/*
 *  PlugParametersTableItem.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PLUGParametersTABLEITEM_INCLUDED
#define PLUGParametersTABLEITEM_INCLUDED

#include <qtable.h>

class AbstractPlug;

class PlugParametersTableItem : public QTableItem
{
public:  
  PlugParametersTableItem(AbstractPlug *plug, QTable *table, EditType et, const QString &text);
  virtual ~PlugParametersTableItem();

protected:
  AbstractPlug *_plug;
};
                               
#endif                               
