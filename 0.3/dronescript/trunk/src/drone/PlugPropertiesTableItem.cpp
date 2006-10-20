#include "PlugPropertiesTableItem.h"
#include "AbstractPlug.h"

PlugPropertiesTableItem::PlugPropertiesTableItem(AbstractPlug *plug, QTable * table, EditType et, const QString &text) :
  QTableItem(table, et, text),
  _plug(plug)
{

}

PlugPropertiesTableItem::~PlugPropertiesTableItem()
{

}


