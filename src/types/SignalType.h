#ifndef SIGNALTYPE_INCLUDED
#define SIGNALTYPE_INCLUDED

#include "VectorType.h"
#include "Engine.h"

typedef float Signal_T;
typedef Signal_T* Signal_T_ptr;

#define clipSignal(sig) ((sig < -1.0f) ? -1.0f : ((sig > 1.0f) ? 1.0f : sig))
#define signalToUChar(sig) ((unsigned char)((sig+1.0f)*128.0f))
#define signalToFrequency(sig) (sig*10000.0f)

class SignalType : public VectorType<Signal_T>
{
public:
  SignalType(Signal_T fillValue = 0.0f)
  {
    resize(Engine::signalInfo().blockSize());
    std::fill(begin(), end(), fillValue);
  }

  virtual ~SignalType(){}

  std::string name() const { return "SignalType";}
  QColor color() const { return QColor(249,169,7);}
};

#endif 
