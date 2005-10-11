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
#include "Gear.h"
#include "GearGui.h"

#include "PlugPropertiesTableItemValue.h"
#include "ValueType.h"
//#include "PlugPropertiesTableItemString.h"



PlugPropertiesTable::PlugPropertiesTable(QWidget *parent) :
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
  header->setLabel( 0, QObject::tr("Property"), 90);
  header->setLabel( 1, QObject::tr("Value"), 90);            
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
  if (gear==NULL)
  {
    std::cout << "warning, null passed to refresh plug properties table" << std::endl;  
    return;
  }

  int nbRows = numRows();
  for(int i=0; i<nbRows;i++)
    removeRow(i);

  _gear=gear;

  std::list<AbstractPlug*> inputs;
  _gear->getInputs(inputs);
  setNumRows(inputs.size());
  int row=0;
  for (std::list<AbstractPlug*>::iterator it = inputs.begin(); it != inputs.end(); ++it,row++)
    insertPlug(*it,row);
  
}

void PlugPropertiesTable::insertPlug(AbstractPlug *plug, int row)
{
  if(!plug)
    return;

  if (plug->abstractType()->typeName() == ValueType::TYPENAME)
  {
    PlugPropertiesTableItemValue *valueItem = new PlugPropertiesTableItemValue(plug, this, QTableItem::WhenCurrent);
    setText(row,0,plug->name());
    setItem(row,1,valueItem);
  }
  
}
