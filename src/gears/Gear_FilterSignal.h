#ifndef GEAR_FILTERSIGNAL_INCLUDED
#define GEAR_FILTERSIGNAL_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "FrequencyAnalyzer.h"

class PlugSignal;

class Gear_FilterSignal : public Gear  
{
public:
  Gear_FilterSignal(Engine *engine, std::string name);
  virtual ~Gear_FilterSignal();

  void runAudio();

  PlugIn<SignalType>* AUDIO_IN(){return _AUDIO_IN;};    
  PlugOut<SignalType>* AUDIO_OUT(){return _AUDIO_OUT;};

  bool ready();
  void init();

private:
  PlugIn<SignalType> *_AUDIO_IN;
  PlugOut<SignalType> *_AUDIO_OUT;

  SignalType _convolutionShape;
  FrequencyAnalyser *_freqAnalyzer;

};

#endif 
