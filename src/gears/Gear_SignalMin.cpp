#include "Gear_SignalMin.h"
#include "Math.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_SignalMin, Gear_SignalMin, "SignalMin")

Gear_SignalMin::Gear_SignalMin(Engine *engine, std::string name)
  : GearConverter<SignalType, ValueType>(engine, "SignalMin", name)
{
}

void Gear_SignalMin::convert()
{
  _PLUG_OUT->type()->setValue( min(_PLUG_IN->type()->data(), _PLUG_IN->type()->size()) );
}
