#ifndef GEAR_OSC_INCLUDED
#define GEAR_OSC_INCLUDED


#include "Gear.h"
#include "SignalType.h"

class Gear_Osc : public Gear
{
public:
  Gear_Osc(Engine *engine, std::string name);
  virtual ~Gear_Osc();

  void runAudio();

  bool ready();

private:
  PlugIn<SignalType> *_PARAM_FREQ, *_PARAM_AMP;
  PlugOut<SignalType> *_AUDIO_OUT;
  Time_T _currentTime;

};

#endif 
