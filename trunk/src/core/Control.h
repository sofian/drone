#ifndef CONTROL_INCLUDED
#define CONTROL_INCLUDED

#include <qwidget.h>

class ControlPanel;
class Gear;

class Control : public QWidget
{
public:  
  Control(Gear *parentGear, ControlPanel *parentControlPanel);
  ~Control();

  virtual void onUpdate(){}
  
protected:
  ControlPanel *_parentControlPanel;
  Gear* _parentGear;
};

#endif
