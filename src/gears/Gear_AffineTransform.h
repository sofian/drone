#ifndef GEAR_AFFINETRANSFORM_INCLUDED
#define GEAR_AFFINETRANSFORM_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "ValueType.h"

class PlugSignal;

class Gear_AffineTransform : public Gear  
{
public:
  Gear_AffineTransform(Engine *engine, std::string name);
  virtual ~Gear_AffineTransform();

  void runVideo();

  bool ready();

private:
  PlugIn<ValueType> *_VALUE_IN,*_SCALE,*_OFFSET;
  PlugOut<ValueType> *_VALUE_OUT;

};

#endif 
