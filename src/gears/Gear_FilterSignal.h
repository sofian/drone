#ifndef GEAR_FILTERSIGNAL_INCLUDED
#define GEAR_FILTERSIGNAL_INCLUDED

#include "Gear.h"
#include "FrequencyAnalyzer.h"

class PlugSignal;

class Gear_FilterSignal : public Gear  
{
public:
  Gear_FilterSignal(Engine *engine, std::string name);
  virtual ~Gear_FilterSignal();

  void runAudio();

  PlugSignalIn* AUDIO_IN(){return _AUDIO_IN;};    
  PlugSignalOut* AUDIO_OUT(){return _AUDIO_OUT;};

  bool ready();
  void init();

private:
  PlugSignalIn *_AUDIO_IN;
  PlugSignalOut *_AUDIO_OUT;

  Signal_T *_convolutionShape;
  FrequencyAnalyser *_freqAnalyzer;

};

#endif 
