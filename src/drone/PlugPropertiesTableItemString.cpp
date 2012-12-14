#include "PlugPropertiesTableItemString.h"
#include "AbstractPlug.h"
#include "StringType.h"

PlugPropertiesTableItemString::PlugPropertiesTableItemString(AbstractPlug *plug, Q3Table * table, EditType et) :
  PlugPropertiesTableItem(plug, table, et, static_cast<StringType*>(plug->abstractDefaultType())->value().c_str()),
  _lineEdit(NULL)
{
  setReplaceable(false);  
}

PlugPropertiesTableItemString::~PlugPropertiesTableItemString()
{

}

QWidget *PlugPropertiesTableItemString::createEditor() const
{
  std::cout << "createEditor" << std::endl;
  //create a lineedit
  ((PlugPropertiesTableItemString*)this)->_lineEdit = new QLineEdit(table()->viewport());
  StringType *data = static_cast<StringType*>(_plug->abstractDefaultType());
  
  _lineEdit->setText(data->value().c_str());  
  
  QObject::connect(_lineEdit, SIGNAL( textChanged(const QString&)), table(), SLOT( doValueChanged()) );
  return _lineEdit;
}

void PlugPropertiesTableItemString::setContentFromEditor(QWidget *w)
{
  std::cout << "setcontentfromeditor" << std::endl;
  if (w->inherits("QLineEdit"))
    setText(((QLineEdit*)w)->text());
  else
    Q3TableItem::setContentFromEditor(w);
}

void PlugPropertiesTableItemString::setText(const QString &s)
{
  std::cout << "settext" << std::endl;
  StringType *data = static_cast<StringType*>(_plug->abstractDefaultType());
  data->setValue(s.toStdString());
  
  if (_lineEdit)
    _lineEdit->setText(s);
  
  Q3TableItem::setText(s);
}


