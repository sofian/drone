/*
 *  PlugListView.cpp
 *  drone
 *
 *  Created by foogy on 20/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>
#include "PlugListView.h"
#include "PlugListViewItem.h"
#include "Gear.h"
#include "GearGui.h"

PlugListView::PlugListView(QWidget *parent) :
	QListView(parent),
	_gear(NULL)
{
	
	addColumn(".",15);
	addColumn("Plug");
	addColumn("Value");
	setRootIsDecorated(true);
	setResizeMode(QListView::LastColumn);
	setColumnWidthMode(0, QListView::Manual);
	setColumnWidthMode(1, QListView::Manual);
	setColumnWidthMode(2, QListView::Manual);
	setAllColumnsShowFocus(true);

	setMinimumWidth(175);
	setTreeStepSize(10);	
}

void PlugListView::slotGearSelected(GearGui *gearGui)
{
	if (gearGui==NULL)
	{
		std::cout<< "NULL GearGui selected?!" << std::endl;
		return;
	}
		
	refresh(gearGui->gear());
}

void PlugListView::refresh(Gear *gear)
{
  if (gear==NULL)
	{
		std::cout << "warning, null passed to refresh plug listview" << std::endl;	
		return;
	}
	
	_gear=gear;
	
	std::list<AbstractPlug*> inputs;
  _gear->getInputs(inputs);
  for (std::list<AbstractPlug*>::iterator it = inputs.begin(); it != inputs.end(); ++it)
  {
		PlugListViewItem *plugListViewItem = new PlugListViewItem(this, *it);
		insertItem(plugListViewItem);
  }
	
}
