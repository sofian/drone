#ifndef GEAR_OSC_INCLUDED
#define GEAR_OSC_INCLUDED


#include "PlugSignal.h"
#include "Gear.h"

class Gear_Osc : public Gear
{
public:
  Gear_Osc(Engine *engine, std::string name);
  virtual ~Gear_Osc();

  void runAudio();

  PlugSignalOut* AUDIO_OUT(){return _AUDIO_OUT;};
  PlugSignalIn* PARAM_AMP(){return _PARAM_AMP;};
  PlugSignalIn* PARAM_FREQ(){return _PARAM_FREQ;};

  bool ready();

private:
  PlugSignalIn *_PARAM_FREQ, *_PARAM_AMP;
  PlugSignalOut *_AUDIO_OUT;
  Time_T _currentTime;

};

#endif 
