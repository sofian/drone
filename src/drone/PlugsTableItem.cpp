#include "PlugsTableItem.h"
#include "AbstractPlug.h"

PlugsTableItem::PlugsTableItem(AbstractPlug *plug, QTable * table, EditType et, const QString &text) :
  QTableItem(table, et, text),
  _plug(plug)
{

}

PlugsTableItem::~PlugsTableItem()
{

}


