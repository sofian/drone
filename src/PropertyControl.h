#ifndef PROPERTYCONTROL_INCLUDED
#define PROPERTYCONTROL_INCLUDED

#include "Properties.h"
#include <qwidget.h>
#include <qlayout.h>


class PropertyControl : public QWidget
{
public:
    PropertyControl(QWidget *parent, Property *property);
    
    virtual void save()=0;
    
protected:
    QWidget *_parent;

    Property *_property;
};

      
#endif
