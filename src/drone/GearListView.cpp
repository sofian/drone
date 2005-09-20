/*
 *  GearListView.cpp
 *  drone
 *
 *  Created by foogy on 12/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#include "GearListView.h"
#include "GearMaker.h"
#include "GearClassification.h"
#include <qdragobject.h>
#include "draggear.xpm"


GearListView::GearListView(QWidget *parent) :
  QListView(parent)
{
    addColumn("Gears");
    setAcceptDrops(true);
    setRootIsDecorated(true);
    setResizeMode(QListView::AllColumns);
    setColumnWidthMode(0, QListView::Manual);
    setMinimumWidth(175);
    setTreeStepSize(10);

}

/**
* create the GearListTree by consulting the gearsInfos in the GearMaker registry
 */
void GearListView::create()
{
  std::vector<const GearInfo*> gearsInfo;
  
  //get all gearsInfo from the GearMaker
  GearMaker::getAllGearsInfo(gearsInfo);
  
  for (std::vector<const GearInfo*>::iterator it = gearsInfo.begin(); it != gearsInfo.end(); ++it)
  {
    QListViewItem *parentItem=NULL;
    QListViewItem *rootItem=NULL;
    
    if ((*it)->classification)
    {     
      std::vector<std::string> path = (*it)->classification->path();
            
      //search or create the root for this gear
      parentItem=firstChild();
      while(parentItem!=NULL && parentItem->text(0) != path[0])
        parentItem=parentItem->nextSibling();
      
      if (parentItem==NULL)
        parentItem = new QListViewItem(this, path[0]);

      parentItem->setOpen(true);
      //search or create subItems path for this gear
      for (std::vector<std::string>::iterator it2 = path.begin()+1; it2!=path.end();++it2)
      {
        rootItem=parentItem;
        parentItem=parentItem->firstChild();

        while(parentItem!=NULL && parentItem->text(0) != *it2)
          parentItem=parentItem->nextSibling();
        
        if (parentItem==NULL)
            parentItem = new QListViewItem(rootItem, *it2);
      }
    }
    
    //insert the gear name in the correct node
    QListViewItem *gearItem = new QListViewItem(parentItem, (*it)->name);
    gearItem->setDragEnabled(true);
  }
}

QDragObject *GearListView::dragObject()
{
  QDragObject *d = new QTextDrag(currentItem()->text(0), this);
  d->setPixmap(QPixmap(draggear));
  return d;  
}