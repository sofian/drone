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
#include <QMimeData>
#include <qdatastream>
#include <qbytearray>
//Added by qt3to4:
#include <QPixmap>
#include "draggear.xpm"


GearTreeView::GearTreeView(QWidget *parent) :
  QTreeWidget(parent)
{
  setColumnCount(1);
  setDragEnabled(true);
  setDragDropMode(QAbstractItemView::DragOnly);
  setDropIndicatorShown (true);
  /*
  
  addColumn("Gears");
    setAcceptDrops(true);
    setRootIsDecorated(true);
    setResizeMode(Q3ListView::AllColumns);
    setColumnWidthMode(0, Q3ListView::Manual);
    setMinimumWidth(200);
    setMinimumHeight(450);
    setTreeStepSize(10);
*/
}





void GearTreeView::mousePressEvent(QMouseEvent *event)
 {
     QTreeWidgetItem *item = itemAt(event->pos());
     if (!item)
         return;

     QMimeData *mimeData = new QMimeData;
     mimeData->setText(item->text(1));
     
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
     items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(gi->name()))));
  }
  insertTopLevelItems(0, items);
  expandAll();
  
  
  
/*  foreach(GearInfo* gi, gearsInfo)
   {
    Q3ListViewItem *parentItem=NULL;
    Q3ListViewItem *rootItem=NULL;
    
    if ((*it)->classification)
    {     
      std::vector<std::string> path = (*it)->classification->path();
            
      //search or create the root for this gear
      parentItem=firstChild();
      while(parentItem!=NULL && parentItem->text(0) != path[0].c_str())
        parentItem=parentItem->nextSibling();
      
      if (parentItem==NULL)
        parentItem = new Q3ListViewItem(this, path[0].c_str());

      parentItem->setOpen(true);
      //search or create subItems path for this gear
      for (std::vector<std::string>::iterator it2 = path.begin()+1; it2!=path.end();++it2)
      {
        rootItem=parentItem;
        parentItem=parentItem->firstChild();

        while(parentItem!=NULL && parentItem->text(0) != it2->c_str())
          parentItem=parentItem->nextSibling();
        
        if (parentItem==NULL)
            parentItem = new Q3ListViewItem(rootItem, it2->c_str());
      }
    }
    
    //insert the gear name in the correct node
  //  Q3ListViewItem *gearItem = new Q3ListViewItem(parentItem, gi->name());
  //  gearItem->setDragEnabled(true);
  }*/
}

Q3DragObject *GearTreeView::dragObject()
{
  Q3DragObject *d = new Q3TextDrag(currentItem()->text(0), this);
  d->setPixmap(QPixmap(draggear));
  return d;  
}
