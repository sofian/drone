#ifndef GEAR_SIMPLE_DELAY_INCLUDED
#define GEAR_SIMPLE_DELAY_INCLUDED

#include "PlugSignal.h"
#include "Gear.h"

class Gear_SimpleDelay : public Gear
{
public:
  Gear_SimpleDelay(Engine *engine, std::string name);
  virtual ~Gear_SimpleDelay();

  void runAudio();

  PlugSignalOut* AUDIO_OUT(){return _AUDIO_OUT;};
  PlugSignalIn* AUDIO_IN(){return _AUDIO_IN;};
  PlugSignalIn* PARAM_TIME(){return _PARAM_TIME;};
  PlugSignalIn* PARAM_FEEDBACK(){return _PARAM_FEEDBACK;};

  bool ready();
private:
  PlugSignalIn *_PARAM_FEEDBACK, *_PARAM_TIME,*_AUDIO_IN;
  PlugSignalOut *_AUDIO_OUT;

  bool _SmoothTime;
  Time_T _MaxTime;

};

#endif
