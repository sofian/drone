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
#include <q3dragobject.h>
//Added by qt3to4:
#include <QPixmap>
#include "draggear.xpm"


GearListView::GearListView(QWidget *parent) :
  Q3ListView(parent)
{
    addColumn("Gears");
    setAcceptDrops(true);
    setRootIsDecorated(true);
    setResizeMode(Q3ListView::AllColumns);
    setColumnWidthMode(0, Q3ListView::Manual);
    setMinimumWidth(200);
    setMinimumHeight(450);
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
    Q3ListViewItem *gearItem = new Q3ListViewItem(parentItem, (*it)->name.c_str());
    gearItem->setDragEnabled(true);
  }
}

Q3DragObject *GearListView::dragObject()
{
  Q3DragObject *d = new Q3TextDrag(currentItem()->text(0), this);
  d->setPixmap(QPixmap(draggear));
  return d;  
}
