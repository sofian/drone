#include "PropertyControl.h"
#include <qpushbutton.h>

PropertyControl::PropertyControl(QWidget *parent, Property *property) :
    QWidget(parent),
    _parent(parent),
    _property(property)
{
}


