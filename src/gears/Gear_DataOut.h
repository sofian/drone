#ifndef GEAR_DATAOUT_INCLUDED
#define GEAR_DATAOUT_INCLUDED

#include <fstream>
#include "SignalType.h"
#include "Gear.h"

class PlugSignal;

class Gear_DataOut : public Gear
{
public:
  Gear_DataOut(Engine *engine, std::string name);
  virtual ~Gear_DataOut();

  void runAudio();  
  bool ready();

private:
  std::ofstream _FileOut;
  PlugIn<SignalType> *_AUDIO_IN;

};

#endif
