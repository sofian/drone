#include "GearControl.h"

GearControl::GearControl(QString type) :
  Gear(type),
  _control(NULL)
{

}

Control* GearControl::createControl(ControlPanel* parent)
{
  _control = internalCreateControl(parent);
  return _control;
}
