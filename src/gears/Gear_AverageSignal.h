#ifndef GEAR_AVERAGESIGNAL_INCLUDED
#define GEAR_AVERAGESIGNAL_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "CircularBufferSignal.h"


class PlugSignal;

class Gear_AverageSignal : public Gear  
{
public:
  Gear_AverageSignal(Engine *engine, std::string name);
  virtual ~Gear_AverageSignal();

  void runAudio();  bool ready();

private:
  PlugIn<SignalType> *_AUDIO_IN;
  PlugOut<SignalType> *_AUDIO_OUT;

  float  _totalSignal;
  float  _nbSamples;
  float  _average;

  CircularBufferSignal *_cbAudioIn;

};

#endif 
