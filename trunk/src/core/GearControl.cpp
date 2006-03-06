#include "GearControl.h"

GearControl::GearControl(Schema *parentSchema, std::string type, std::string uniqueName) :
  Gear(parentSchema,type,uniqueName),
  _control(NULL)
{

}

Control* GearControl::createControl(ControlPanel* parent)
{
  _control = internalCreateControl(parent);
  return _control;
}
