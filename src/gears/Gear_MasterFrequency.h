#ifndef GEAR_MASTERFREQUENCY_INCLUDED
#define GEAR_MASTERFREQUENCY_INCLUDED

#include "Gear.h"
#include "FrequencyAnalyzer.h"

class PlugSignal;

class Gear_MasterFrequency : public Gear  
{
public:
  Gear_MasterFrequency(Engine *engine, std::string name);
  virtual ~Gear_MasterFrequency();

  void runAudio();

  PlugSignalIn* AUDIO_IN(){return _AUDIO_IN;};    
  PlugSignalOut* AUDIO_OUT(){return _AUDIO_OUT;};

  void init();

  bool ready();

private:
  PlugSignalIn *_AUDIO_IN;
  PlugSignalOut *_AUDIO_OUT;

  FrequencyAnalyser *_freqAnalyser;


};

#endif 
