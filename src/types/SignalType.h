#ifndef SIGNALTYPE_INCLUDED
#define SIGNALTYPE_INCLUDED

#include "VectorType.h"
#include "Engine.h"

class SignalType : public VectorType<float>
{
public:
  SignalType(float fillValue = 0.0f)
  {
    resize(Engine::signalInfo().blockSize());
    std::fill(begin(), end(), fillValue);
  }

  virtual ~SignalType(){}

  std::string name() const { return "SignalType";}
  QColor color() const { return QColor(249,169,7);}
};

#endif 
