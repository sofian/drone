#ifndef GEAR_ENVELOPPE_INCLUDED
#define GEAR_ENVELOPPE_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "ValueType.h"
#include "Signal.h"

class PlugSignal;

class Gear_Enveloppe : public Gear  
{
public:
	Gear_Enveloppe(Engine *engine, std::string name);
	virtual ~Gear_Enveloppe();

  void runAudio();
  
  bool ready();

  virtual void prePlay();

private:
  PlugIn<SignalType> *_AUDIO_IN;
  PlugIn<ValueType> *_PARAM_RELEASE, *_PARAM_ATTACK;
  PlugOut<SignalType> *_AUDIO_OUT; 
  
  Signal_T _lastenvel,_envel;
  Signal_T _envin;

};

#endif 
