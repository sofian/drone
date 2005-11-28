#include "PlugParametersTableItem.h"
#include "AbstractPlug.h"

PlugParametersTableItem::PlugParametersTableItem(AbstractPlug *plug, QTable * table, EditType et, const QString &text) :
  QTableItem(table, et, text),
  _plug(plug)
{

}

PlugParametersTableItem::~PlugParametersTableItem()
{

}


