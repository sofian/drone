#ifndef GEAR_SIGNALMIN_INCLUDED
#define GEAR_SIGNALMIN_INCLUDED

#include "GearConverter.h"
#include "types/SignalType.h"
#include "types/ValueType.h"

class Gear_SignalMin : public GearConverter<SignalType, ValueType>
{
public:
  Gear_SignalMin(Engine *engine, std::string name);
  virtual ~Gear_SignalMin() {}

  void convert();
};


#endif
