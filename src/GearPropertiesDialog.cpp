#include <qlayout.h>
#include <qpushbutton.h>

#include "GearPropertiesDialog.h"
#include "Gear.h"
#include "PropertyControl.h"
#include "PropertyControlString.h"
#include "PropertyControlFilename.h"
#include "PropertyControlBool.h"



GearPropertiesDialog::GearPropertiesDialog(QWidget *parent, Gear *gear) :
  QDialog(parent, "Properties", true),
  _gear(gear)
{
  setCaption("Properties");
  _verticalLayout = new QVBoxLayout(this, 12, 12, "layout");

  std::vector<Property*> properties;
  _gear->settings().getAll(&properties);


  //create all PropertieControls for the gear
  for (std::vector<Property*>::iterator it=properties.begin(); it!=properties.end(); ++it)
    addControl((*it));

  //ok cancels
  QHBoxLayout *hLayout = new QHBoxLayout(2);
  _okButton = new QPushButton("Ok", this);
  hLayout->addWidget(_okButton);
  QObject::connect(_okButton, SIGNAL(clicked()), this, SLOT(slotOK()));                
  _cancelButton =  new QPushButton("Cancel", this);
  QObject::connect(_cancelButton, SIGNAL(clicked()), this, SLOT(accept()));               
  hLayout->addWidget(_cancelButton);
  _verticalLayout->addLayout(hLayout);

}

GearPropertiesDialog::~GearPropertiesDialog()
{

}

void GearPropertiesDialog::slotOK()
{    
  //save all
  for (std::vector<PropertyControl*>::iterator it=_propertyControls.begin(); it!=_propertyControls.end(); ++it)
  {
    (*it)->save();
  }

  //tell the gear to update his settings
  _gear->updateSettings();

  accept();
}

void GearPropertiesDialog::addControl(Property *property)
{
  PropertyControl *propertyControl=NULL;

  switch (property->type())
  {
  case Property::STRING:
    propertyControl = new PropertyControlString(this, property);
    break;
  case Property::FILENAME:
    propertyControl = new PropertyControlFilename(this, property);
    break;
  case Property::INT:
    propertyControl = new PropertyControlString(this, property);
    break;
  case Property::FLOAT:
    propertyControl = new PropertyControlString(this, property);
    break;

  case Property::BOOL:
    propertyControl = new PropertyControlBool(this, property);
    break;

  default:
    propertyControl = new PropertyControlString(this, property);
  }

  _propertyControls.push_back(propertyControl);
  _verticalLayout->addWidget(propertyControl);    


}
