#ifndef GEAR_GAIN_INCLUDED
#define GEAR_GAIN_INCLUDED

#include "Gear.h"
#include "SignalType.h"

class PlugSignal;

class Gear_Gain : public Gear  
{
public:
  Gear_Gain(Engine *engine, std::string name);
  virtual ~Gear_Gain();

  void runAudio();

  PlugIn<SignalType>* AUDIO_IN(){return _AUDIO_IN;};
  PlugIn<SignalType>* PARAM_GAIN(){return _PARAM_GAIN;};
  PlugOut<SignalType>* AUDIO_OUT(){return _AUDIO_OUT;};

  bool ready();

private:
  PlugIn<SignalType> *_AUDIO_IN, *_PARAM_GAIN;
  PlugOut<SignalType> *_AUDIO_OUT;

};

#endif 
