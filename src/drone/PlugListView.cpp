/*
 *  PlugPropertiesTable.cpp
 *  drone
 *
 *  Created by foogy on 20/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>
#include "PlugPropertiesTable.h"
#include "PlugPropertiesTableItem.h"
#include "Gear.h"
#include "GearGui.h"

PlugPropertiesTable::PlugPropertiesTable(QWidget *parent) :
	QTable(parent),
	_gear(NULL)
{
	
	QHeader *header = horizontalHeader();
	header->setLabel( 0, QObject::tr( "V" ), 40 );
	header->setLabel( 1, QObject::tr( "Property" ) );
	header->setLabel( 2, QObject::tr( "Value" ) );    
	setColumnMovingEnabled(TRUE);
		
    //hide vertical labels
    verticalHeader()->hide();
    setLeftMargin(0);

	setMinimumWidth(175);
}

void PlugPropertiesTable::slotGearSelected(GearGui *gearGui)
{
	if (gearGui==NULL)
	{
		std::cout<< "NULL GearGui selected?!" << std::endl;
		return;
	}
		
	refresh(gearGui->gear());
}

void PlugPropertiesTable::refresh(Gear *gear)
{
/*
  if (gear==NULL)
	{
		std::cout << "warning, null passed to refresh plug listview" << std::endl;	
		return;
	}

	clear();
	_gear=gear;
	
	std::list<AbstractPlug*> inputs;
  _gear->getInputs(inputs);
  for (std::list<AbstractPlug*>::iterator it = inputs.begin(); it != inputs.end(); ++it)
  {
		PlugPropertiesTableItem *PlugPropertiesTableItem = new PlugPropertiesTableItem(this, *it);
		insertItem(PlugPropertiesTableItem);
  }
*/	
}
