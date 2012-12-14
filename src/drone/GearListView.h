/*
 *  GearListView.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GEARLISTVIEW_INCLUDED
#define GEARLISTVIEW_INCLUDED

#include <q3listview.h>

class GearListView : public Q3ListView
{
public:
  GearListView(QWidget *parent);
  
  void create();    
protected:
  Q3DragObject *dragObject();
};

#endif
