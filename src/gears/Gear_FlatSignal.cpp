#include "Gear_FlatSignal.h"
#include "Math.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_FlatSignal, Gear_FlatSignal, "FlatSignal")

Gear_FlatSignal::Gear_FlatSignal(Engine *engine, std::string name)
  : GearConverter<ValueType, SignalType>(engine, "FlatSignal", name)
{
  _PLUG_IN->hintType()->setValue(0.0f);
  _PLUG_IN->hintType()->setMinValue(-1.0f);
  _PLUG_IN->hintType()->setMaxValue(1.0f);
}

void Gear_FlatSignal::convert()
{
  _PLUG_OUT->type()->fill(_PLUG_IN->type()->value());
}
