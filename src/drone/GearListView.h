/*
 *  GearListView.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <qlistview.h>

class GearListView : public QListView
{
public:
  GearListView(QWidget *parent);
  
  void create();    
protected:
  QDragObject *dragObject();
};