#ifndef PROPERTYCONTROLBOOL_INCLUDED
#define PROPERTYCONTROLBOOL_INCLUDED

#include "PropertyControl.h"

class QCheckBox;


class PropertyControlBool : public PropertyControl
{
public:
  PropertyControlBool(QWidget *parent, Property *property);
  virtual ~PropertyControlBool();

  void save();


private:

  QHBoxLayout *_hLayout;
  QCheckBox *_checkBox;

};

#endif
