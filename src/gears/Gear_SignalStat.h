#ifndef GEAR_SIGNALSTAT_INCLUDED
#define GEAR_SIGNALSTAT_INCLUDED

#include "GearConverter.h"
#include "types/SignalType.h"
#include "types/ValueType.h"

class Gear_SignalStat : public GearConverter<SignalType, ValueType>
{
public:
  Gear_SignalStat(Engine *engine, std::string name);
  virtual ~Gear_SignalStat() {}

  void convert();

protected:
  PlugIn<ValueType> *_FUNC;
};


#endif
