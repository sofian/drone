#include "PlugPropertiesTableItem.h"
#include "AbstractPlug.h"

PlugPropertiesTableItem::PlugPropertiesTableItem(AbstractPlug *plug, Q3Table * table, EditType et, const QString &text) :
  Q3TableItem(table, et, text),
  _plug(plug)
{

}

PlugPropertiesTableItem::~PlugPropertiesTableItem()
{

}


