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
#include "PlugPropertiesTableItemString.h"

#include "ValueType.h"
#include "StringType.h"


PlugPropertiesTable::PlugPropertiesTable(QWidget *parent) :
Q3Table(parent),
_gear(NULL)
{

  //hide vertical labels
  verticalHeader()->hide();
  setLeftMargin(0);
  setMinimumWidth(185);
  setColumnReadOnly(0,true);


  setNumCols(2);
  setNumRows(0);
  Q3Header *header = horizontalHeader();
  header->setLabel( 0, QObject::tr("Property"), 90);
  header->setLabel( 1, QObject::tr("Value"), 90);            
  setColumnStretchable(1,true);
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

/*
  int nbRows = numRows();
  for(int i=0; i<nbRows;i++)
    removeRow(i);
*/

  setNumRows(0);
  _gear=gear;

  QList<AbstractPlug*> inputs;
  _gear->getInputs(inputs);
  setNumRows(inputs.count());
  int row=0;
  foreach (AbstractPlug* ap,inputs)
  {
    if (!ap->connected())
    	insertPlug(ap,row);    
  }
}

void PlugPropertiesTable::insertPlug(AbstractPlug *plug, int row)
{
  if(!plug)
    return;

  if (plug->abstractType()->typeName() == ValueType().typeName())
  {
    PlugPropertiesTableItemValue *valueItem = new PlugPropertiesTableItemValue(plug, this, Q3TableItem::WhenCurrent);
    setText(row,0,plug->name());
    setItem(row,1,valueItem);
  } 
  else if (plug->abstractType()->typeName() == StringType().typeName())
  {
    PlugPropertiesTableItemString *stringItem = new PlugPropertiesTableItemString(plug, this, Q3TableItem::WhenCurrent);
    setText(row,0,plug->name());
    setItem(row,1,stringItem);  
  }
}
