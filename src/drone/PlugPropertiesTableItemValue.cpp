#include "PlugPropertiesTableItemValue.h"
#include "AbstractPlug.h"
#include "ValueType.h"

PlugPropertiesTableItemValue::PlugPropertiesTableItemValue(AbstractPlug *plug, Q3Table * table, EditType et) :
  PlugPropertiesTableItem(plug, table, et, QString::number(static_cast<ValueType*>(plug->abstractDefaultType())->value())),
  _lineEdit(NULL)
{
  setReplaceable(false);  
}

PlugPropertiesTableItemValue::~PlugPropertiesTableItemValue()
{

}

QWidget *PlugPropertiesTableItemValue::createEditor() const
{
  std::cout << "createEditor" << std::endl;
  //create a lineedit
  ((PlugPropertiesTableItemValue*)this)->_lineEdit = new QLineEdit(table()->viewport());
  _lineEdit->setText(QString::number(static_cast<ValueType*>(_plug->abstractDefaultType())->value()));  
  
  QObject::connect(_lineEdit, SIGNAL( textChanged(const QString&)), table(), SLOT( doValueChanged()) );
  return _lineEdit;
}

void PlugPropertiesTableItemValue::setContentFromEditor(QWidget *w)
{
  std::cout << "setcontentfromeditor" << std::endl;
  if (w->inherits("QLineEdit"))
    setText(((QLineEdit*)w)->text());
  else
    Q3TableItem::setContentFromEditor(w);
}

void PlugPropertiesTableItemValue::setText(const QString &s)
{
  std::cout << "settext" << std::endl;
  ValueType *data = static_cast<ValueType*>(_plug->abstractDefaultType());
  data->setValue(s.toFloat());
  
  if (_lineEdit)
    _lineEdit->setText(s);
  
  Q3TableItem::setText(s);
}


