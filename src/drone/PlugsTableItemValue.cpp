#include "PlugsTableItemValue.h"
#include "AbstractPlug.h"
#include "ValueType.h"

PlugsTableItemValue::PlugsTableItemValue(AbstractPlug *plug, QTable * table, EditType et) :
  PlugsTableItem(plug, table, et, QString::number(static_cast<ValueType*>(plug->abstractDefaultType())->value())),
  _lineEdit(NULL)
{
  setReplaceable(false);  
}

PlugsTableItemValue::~PlugsTableItemValue()
{

}

QWidget *PlugsTableItemValue::createEditor() const
{
  std::cout << "createEditor" << std::endl;
  //create a lineedit
  ((PlugsTableItemValue*)this)->_lineEdit = new QLineEdit(table()->viewport());
  _lineEdit->setText(QString::number(static_cast<ValueType*>(_plug->abstractDefaultType())->value()));  
  
  QObject::connect(_lineEdit, SIGNAL( textChanged(const QString&)), table(), SLOT( doValueChanged()) );
  return _lineEdit;
}

void PlugsTableItemValue::setContentFromEditor(QWidget *w)
{
  std::cout << "setcontentfromeditor" << std::endl;
  if (w->inherits("QLineEdit"))
    setText(((QLineEdit*)w)->text());
  else
    QTableItem::setContentFromEditor(w);
}

void PlugsTableItemValue::setText(const QString &s)
{
  std::cout << "settext" << std::endl;
  ValueType *data = static_cast<ValueType*>(_plug->abstractDefaultType());
  data->setValue(s.toFloat());
  
  if (_lineEdit)
    _lineEdit->setText(s);
  
  QTableItem::setText(s);
}


