#ifndef GEAR_VALUEFLATSIGNAL_INCLUDED
#define GEAR_VALUEFLATSIGNAL_INCLUDED

#include "GearConverter.h"
#include "types/SignalType.h"
#include "types/ValueType.h"

class Gear_FlatSignal : public GearConverter<ValueType, SignalType>
{
public:
  Gear_FlatSignal(Engine *engine, std::string name);
  virtual ~Gear_FlatSignal() {}

  void convert();
};


#endif
