#ifndef GEAR_SIGNALMEAN_INCLUDED
#define GEAR_SIGNALMEAN_INCLUDED

#include "GearConverter.h"
#include "types/SignalType.h"
#include "types/ValueType.h"

class Gear_SignalMean : public GearConverter<SignalType, ValueType>
{
public:
  Gear_SignalMean(Engine *engine, std::string name);
  virtual ~Gear_SignalMean() {}

  void convert();
};


#endif
