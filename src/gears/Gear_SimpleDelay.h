#ifndef GEAR_SIMPLE_DELAY_INCLUDED
#define GEAR_SIMPLE_DELAY_INCLUDED

#include "Gear.h"
#include "SignalType.h"

class Gear_SimpleDelay : public Gear
{
public:
  Gear_SimpleDelay(Engine *engine, std::string name);
  virtual ~Gear_SimpleDelay();

  void runAudio();

  bool ready();
  
private:
  PlugIn<SignalType> *_PARAM_FEEDBACK, *_PARAM_TIME,*_AUDIO_IN;
  PlugOut<SignalType> *_AUDIO_OUT;

  bool _SmoothTime;
  Time_T _MaxTime;

};

#endif
