#ifndef GEAR_AudioSource_INCLUDED
#define GEAR_AudioSource_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "sndfile.h"

class Gear_AudioSource : public Gear
{
public:
  static const std::string SETTING_FILENAME;

  Gear_AudioSource(Engine *engine, std::string name);
  virtual ~Gear_AudioSource();

  void runAudio();

  PlugOut<SignalType>* AUDIO_OUT(){return _AUDIO_OUT;};

  bool ready();

protected:
  void onUpdateSettings();

private:

  PlugOut<SignalType> *_AUDIO_OUT;
  SNDFILE* _File;
  SF_INFO _SFInfo;
};

#endif