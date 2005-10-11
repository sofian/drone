#include "PlugPropertiesTableItem.h"
#include "AbstractPlug.h"

PlugPropertiesTableItem::PlugPropertiesTableItem(AbstractPlug *plug, QTable * table, EditType et) :
  QTableItem(table, et),
  _plug(plug)
{

}

PlugPropertiesTableItem::~PlugPropertiesTableItem()
{

}


