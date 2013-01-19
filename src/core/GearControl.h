#ifndef GEARCONTROL_INCLUDED
#define GEARCONTROL_INCLUDED

#include "Gear.h"

class Control;


class GearControl : public Gear
{
  Q_OBJECT

public:

  GearControl(QString type);

  virtual ~GearControl(){}
  void setControl(Control* control);
  Control* getControl(){return _control;}
  
protected:
  Control* _control;
  
};

#endif
