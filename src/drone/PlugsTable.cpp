/*
 *  PlugsTable.cpp
 *  drone
 *
 *  Created by foogy on 20/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>
#include "PlugsTable.h"
#include "Gear.h"
#include "GearGui.h"

#include "PlugsTableItemValue.h"
#include "PlugsTableItemString.h"
#include "PlugsTableItemFilename.h"

#include "ValueType.h"
#include "StringType.h"


PlugsTable::PlugsTable(QWidget *parent) :
QTable(parent),
_gear(NULL)
{

  //hide vertical labels
  verticalHeader()->hide();
  setLeftMargin(0);
  setMinimumWidth(185);
  setColumnReadOnly(0,true);


  setNumCols(2);
  setNumRows(0);
  QHeader *header = horizontalHeader();
  header->setLabel( 0, QObject::tr("Parameter"), 90);
  header->setLabel( 1, QObject::tr("Value"), 90);            
  setColumnStretchable(1,true);
}

void PlugsTable::slotGearSelected(GearGui *gearGui)
{
  if (gearGui==NULL)
  {
    std::cout<< "NULL GearGui selected?!" << std::endl;
    return;
  }

  refresh(gearGui->gear());
}

void PlugsTable::refresh(Gear *gear)
{
  if (gear==NULL)
  {
    std::cout << "warning, null passed to refresh plug Parameters table" << std::endl;  
    return;
  }

/*
  int nbRows = numRows();
  for(int i=0; i<nbRows;i++)
    removeRow(i);
*/

  setNumRows(0);
  _gear=gear;

  std::list<AbstractPlug*> inputs;
  _gear->getInputs(inputs);
  setNumRows(inputs.size());
  int row=0;
  for (std::list<AbstractPlug*>::iterator it = inputs.begin(); it != inputs.end(); ++it,row++)
  {
    if (!(*it)->connected())
    	insertPlug(*it,row);    
  }
}

void PlugsTable::insertPlug(AbstractPlug *plug, int row)
{
  if(!plug)
    return;

  if (plug->abstractType()->typeName() == ValueType::TYPENAME)
  {
    PlugsTableItemValue *valueItem = new PlugsTableItemValue(plug, this, QTableItem::WhenCurrent);
		addRow(row, plug, valueItem);
  } 
  else if (plug->abstractType()->typeName() == StringType::TYPENAME)
  {
    StringType *strType = (StringType*)plug->abstractType();
		if (strType->isAFilename())
		{
			PlugsTableItemFilename *filenameItem = new PlugsTableItemFilename(plug, this, QTableItem::WhenCurrent);
			addRow(row, plug, filenameItem);
		}
		else
		{
			PlugsTableItemString *stringItem = new PlugsTableItemString(plug, this, QTableItem::WhenCurrent);
			addRow(row, plug, stringItem);
		}
  }
}

void PlugsTable::addRow(int row, AbstractPlug *plug, QTableItem *tableItem)
{
	setText(row,0,plug->name());
	setItem(row,1,tableItem);  				
}
																																		