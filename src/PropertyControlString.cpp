#include "PropertyControlString.h"
#include <qlineedit.h>
#include <qlabel.h>


PropertyControlString::PropertyControlString(QWidget *parent, Property *property) :
PropertyControl(parent, property)
{
  _hLayout = new QHBoxLayout(this, 2);    
  _hLayout->addWidget(new QLabel(property->name().c_str(), this, ""));
  _lineEdit = new QLineEdit(this);
  _hLayout->addWidget(_lineEdit);

  _lineEdit->setText(property->valueStr().c_str());
}

PropertyControlString::~PropertyControlString()
{
}

void PropertyControlString::save()
{
  _property->valueStr(_lineEdit->text().ascii());
}




