#ifndef GEARCONTROL_INCLUDED
#define GEARCONTROL_INCLUDED

#include "Gear.h"

class Control;


class GearControl : public Gear
{
public:

  GearControl(QString type);

  virtual ~GearControl(){}
  
protected:
};

#endif
