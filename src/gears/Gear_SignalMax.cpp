#include "Gear_SignalMax.h"
#include "Math.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_SignalMax, Gear_SignalMax, "SignalMax")

Gear_SignalMax::Gear_SignalMax(Engine *engine, std::string name)
  : GearConverter<SignalType, ValueType>(engine, "SignalMax", name)
{
}

void Gear_SignalMax::convert()
{
  _PLUG_OUT->type()->setValue( max(_PLUG_IN->type()->data(), _PLUG_IN->type()->size())  );
}
