#ifndef GEARCONTROL_INCLUDED
#define GEARCONTROL_INCLUDED

#include "Gear.h"

class Control;
class ControlPanel;


class GearControl : public Gear
{
public:

  GearControl(QString type);

  virtual ~GearControl(){}
  
  //virtual GearKind kind() const {return CONTROL;}
     
  Control* createControl(ControlPanel* parent);

  Control* getControl(){return _control;}
  
protected:
  
  virtual Control* internalCreateControl(ControlPanel*)=0;

  Control *_control;
};

#endif
