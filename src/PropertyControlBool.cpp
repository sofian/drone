#include "PropertyControlBool.h"
#include <qcheckbox.h>
#include <qlabel.h>


PropertyControlBool::PropertyControlBool(QWidget *parent, Property *property) :
PropertyControl(parent, property)
{
  _hLayout = new QHBoxLayout(this, 2);    
  _hLayout->addWidget(new QLabel(property->name().c_str(), this, ""));
  _checkBox = new QCheckBox(this);
  _hLayout->addWidget(_checkBox);

  _checkBox->setChecked(property->valueBool());
}                       

PropertyControlBool::~PropertyControlBool()
{
}

void PropertyControlBool::save()
{
  _property->valueBool(_checkBox->isChecked());
}




