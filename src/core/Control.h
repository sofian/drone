#ifndef CONTROL_INCLUDED
#define CONTROL_INCLUDED

#include <qgraphicswidget.h>

class ControlPanel;
class Gear;

class Control : public QGraphicsWidget
{
public:  
  Control();
  virtual ~Control();

  virtual void onUpdate(){}
  virtual void setGear(Gear* g){_parentGear=g;}
  virtual void runVideo(){}
  
  // must return the GearControl class (Ex: Gear_PushButton)
  virtual QString getGearType()=0;
protected:
  ControlPanel *_parentControlPanel;
  Gear* _parentGear;
};

#endif
