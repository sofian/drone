#ifndef GEAR_AVERAGESIGNAL_INCLUDED
#define GEAR_AVERAGESIGNAL_INCLUDED

#include "Gear.h"
#include "CircularBufferSignal.h"


class PlugSignal;

class Gear_AverageSignal : public Gear  
{
public:
  Gear_AverageSignal(Engine *engine, std::string name);
  virtual ~Gear_AverageSignal();

  void runAudio();

  PlugSignalIn* AUDIO_IN(){return _AUDIO_IN;};    
  PlugSignalOut* AUDIO_OUT(){return _AUDIO_OUT;};

  bool ready();

private:
  PlugSignalIn *_AUDIO_IN;
  PlugSignalOut *_AUDIO_OUT;

  Signal_T _totalSignal;
  Signal_T _nbSamples;
  Signal_T _average;

  CircularBufferSignal *_cbAudioIn;

};

#endif 
