#ifndef GEAR_MASTERFREQUENCY_INCLUDED
#define GEAR_MASTERFREQUENCY_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "FrequencyAnalyzer.h"

class PlugSignal;

class Gear_MasterFrequency : public Gear  
{
public:
  Gear_MasterFrequency(Engine *engine, std::string name);
  virtual ~Gear_MasterFrequency();

  void runAudio();

  PlugIn<SignalType>* AUDIO_IN(){return _AUDIO_IN;};    
  PlugOut<SignalType>* AUDIO_OUT(){return _AUDIO_OUT;};

  void init();

  bool ready();

private:
  PlugIn<SignalType> *_AUDIO_IN;
  PlugOut<SignalType> *_AUDIO_OUT;

  FrequencyAnalyser *_freqAnalyser;


};

#endif 
