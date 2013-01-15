/*
 *  GearTreeView.cpp
 *  drone
 *
 *  Created by foogy on 12/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#include "GearTreeView.h"
#include "GearMaker.h"
#include "GearClassification.h"
#include <q3dragobject.h>
#include <qmouseevent>
#include <qtreewidgetitem>
#include <QMimeData>
#include <qdatastream>
#include <qbytearray>
//Added by qt3to4:
#include <QPixmap>
#include "draggear.xpm"
#include <qheaderview>

GearTreeView::GearTreeView(QWidget *parent) :
  QTreeWidget(parent)
{
  setColumnCount(1);
  setDragEnabled(true);
  setDragDropMode(QAbstractItemView::DragOnly);
  setDropIndicatorShown (true);
  header()->hide();
  setMinimumWidth(200);
  setMinimumHeight(400);
}





void GearTreeView::mousePressEvent(QMouseEvent *event)
 {
     QTreeWidgetItem *item = itemAt(event->pos());
     if (!item || item->childCount()>0 || event->button() != Qt::LeftButton)
     {
         QTreeView::mousePressEvent(event);
         return;
     }
     
     QMimeData *mimeData = new QMimeData;
     mimeData->setText(item->data(1,Qt::UserRole).toString());
     
     QDrag *drag = new QDrag(this);
     drag->setMimeData(mimeData);
     //drag->setHotSpot(event->pos() - item->pos());

/*     QPixmap tempPixmap = pixmap;
     QPainter painter;
     painter.begin(&tempPixmap);
     painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
     painter.end();

     child->setPixmap(tempPixmap);
*/
     drag->exec();
 }

QTreeWidgetItem* GearTreeView::findClassificationItem(QStringList path, QTreeWidgetItem * parent)
{
  QTreeWidgetItem* base=NULL;
  int childIndex=0;
  
  for(int i=0;i<parent->childCount();i++)
  {
    if(parent->child(i)->text(0) == path[0])
    {
      base=parent->child(i);
      break;
    }
  }
  if(base!=NULL)
    return base;
  else
  {
    base = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(path[0])));
    parent->addChild(base);
    path.removeFirst();
    if(path.isEmpty())
      return base;
    return findClassificationItem(path,base);
  }
}


/**
* create the GearListTree by consulting the gearsInfos in the GearMaker registry
 */
void GearTreeView::create()
{
 QList<GearInfo*> gearsInfo;

  //get all gearsInfo from the GearMaker
  GearMaker::instance()->getAllGearsInfo(gearsInfo);
 
  QList<QTreeWidgetItem *> items;
  foreach(GearInfo* gi, gearsInfo)
  {        
    QTreeWidgetItem* parentItem = findClassificationItem(gi->getClassification(),invisibleRootItem());
    
    QTreeWidgetItem* child = new QTreeWidgetItem((QTreeWidget*)0, QStringList(gi->name()));
    child->setData(1,Qt::UserRole, gi->fullName());
    parentItem->addChild(child);
  }
  expandAll();  
}

