#include "Gear_SignalStat.h"
#include "Math.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_SignalStat, Gear_SignalStat, "SignalStat")

Gear_SignalStat::Gear_SignalStat(Engine *engine, std::string name)
  : GearConverter<SignalType, ValueType>(engine, "SignalStat", name)
{
  addPlug(_FUNC = new PlugIn<ValueType>(this, "Func", new ValueType(0, 0, 6)));

}

void Gear_SignalStat::convert()
{
  int size = _PLUG_IN->type()->size();

  switch((int)_FUNC->type()->value())
  {
  //first of buffer
  case 0:
      _PLUG_OUT->type()->setValue( _PLUG_IN->type()->data()[0]);
      break;
  //average
  case 1:
    //   NOTICE("%f",123.123);// (float)ssize);
      _PLUG_OUT->type()->setValue( sum(_PLUG_IN->type()->data(), size) / size );
      break;
  // min
  case 2:
      _PLUG_OUT->type()->setValue( min(_PLUG_IN->type()->data(), size));
      break;
  // max
  case 3:
      _PLUG_OUT->type()->setValue( max(_PLUG_IN->type()->data(), size));
      break;
  }

}
