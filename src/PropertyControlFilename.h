#ifndef PROPERTYCONTROLFILENAME_INCLUDED
#define PROPERTYCONTROLFILENAME_INCLUDED

#include "PropertyControl.h"

class QLineEdit;
class QPushButton;

class PropertyControlFilename : public PropertyControl
{
Q_OBJECT
public:
    PropertyControlFilename(QWidget *parent, Property *property);
    virtual ~PropertyControlFilename();

    void save();
   
    
public slots:
    void slotBrowseClicked();    

private:
    
    QHBoxLayout *_hLayout;
    QLineEdit *_lineEdit;
    QPushButton *_browseButton;
    

};
      
#endif
