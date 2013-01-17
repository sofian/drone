#ifndef CONTROL_INCLUDED
#define CONTROL_INCLUDED

#include <qgraphicswidget.h>

class ControlPanel;
class Gear;

class Control : public QGraphicsWidget
{
public:  
  Control(Gear *parentGear);
  ~Control();

  virtual void onUpdate(){}
  virtual void setupGear(Gear*){}
  virtual void runVideo(){}
  
  // must return the GearControl class (Ex: Gear_PushButton)
  virtual QString getGearType()=0;
protected:
  ControlPanel *_parentControlPanel;
  Gear* _parentGear;
};

#endif
