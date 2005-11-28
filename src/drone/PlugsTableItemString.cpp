#include "PlugsTableItemString.h"
#include "AbstractPlug.h"
#include "StringType.h"

PlugsTableItemString::PlugsTableItemString(AbstractPlug *plug, QTable * table, EditType et) :
  PlugsTableItem(plug, table, et, static_cast<StringType*>(plug->abstractDefaultType())->value().c_str()),
  _lineEdit(NULL)
{
  setReplaceable(false);  
}

PlugsTableItemString::~PlugsTableItemString()
{

}

QWidget *PlugsTableItemString::createEditor() const
{
  std::cout << "createEditor" << std::endl;
  //create a lineedit
  ((PlugsTableItemString*)this)->_lineEdit = new QLineEdit(table()->viewport());
  StringType *data = static_cast<StringType*>(_plug->abstractDefaultType());
  
  _lineEdit->setText(data->value());  
  
  QObject::connect(_lineEdit, SIGNAL( textChanged(const QString&)), table(), SLOT( doValueChanged()) );
  return _lineEdit;
}

void PlugsTableItemString::setContentFromEditor(QWidget *w)
{
  std::cout << "setcontentfromeditor" << std::endl;
  if (w->inherits("QLineEdit"))
    setText(((QLineEdit*)w)->text());
  else
    QTableItem::setContentFromEditor(w);
}

void PlugsTableItemString::setText(const QString &s)
{
  std::cout << "settext" << std::endl;
  StringType *data = static_cast<StringType*>(_plug->abstractDefaultType());
  data->setValue(s);
  
  if (_lineEdit)
    _lineEdit->setText(s);
  
  QTableItem::setText(s);
}


