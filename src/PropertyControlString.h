#ifndef PROPERTYCONTROLSTRING_INCLUDED
#define PROPERTYCONTROLSTRING_INCLUDED

#include "PropertyControl.h"

class QLineEdit;


class PropertyControlString : public PropertyControl
{
public:
  PropertyControlString(QWidget *parent, Property *property);
  virtual ~PropertyControlString();

  void save();


private:

  QHBoxLayout *_hLayout;
  QLineEdit *_lineEdit;

};

#endif
