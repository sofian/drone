#include "Gear_SignalMean.h"
#include "Math.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_SignalMean, Gear_SignalMean, "SignalMean")

Gear_SignalMean::Gear_SignalMean(Engine *engine, std::string name)
  : GearConverter<SignalType, ValueType>(engine, "SignalMean", name)
{
}

void Gear_SignalMean::convert()
{
  int size = _PLUG_IN->type()->size();
  _PLUG_OUT->type()->setValue( sum(_PLUG_IN->type()->data(), size) / size );
}
