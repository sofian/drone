#include "GearControl.h"

GearControl::GearControl(QString type) :
  Gear(type),
  _control(NULL)
{

}

void GearControl::setControl(Control* control)
{
  _control=control;
}