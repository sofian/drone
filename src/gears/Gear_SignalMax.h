#ifndef GEAR_SIGNALMAX_INCLUDED
#define GEAR_SIGNALMAX_INCLUDED

#include "GearConverter.h"
#include "types/SignalType.h"
#include "types/ValueType.h"

class Gear_SignalMax : public GearConverter<SignalType, ValueType>
{
public:
  Gear_SignalMax(Engine *engine, std::string name);
  virtual ~Gear_SignalMax() {}

  void convert();
};


#endif
