/*
 *  GearTreeView.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GEARTREEVIEW_INCLUDED
#define GEARTREEVIEW_INCLUDED

#include <qtreewidget.h>
class Q3DragObject;

class GearTreeView : public QTreeWidget
{
public:
  GearTreeView(QWidget *parent);
  
  void create();    
protected:
  void mousePressEvent(QMouseEvent *event);

  Q3DragObject *dragObject();
};

#endif
