#ifndef GEAR_GAIN_INCLUDED
#define GEAR_GAIN_INCLUDED

#include "Gear.h"

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
    PlugSignalIn *_AUDIO_IN, *_PARAM_RELEASE,*_PARAM_ATTACK;
	PlugSignalOut *_AUDIO_OUT;
  
  Signal_T _lastenvel,_envel;
  Signal_T _envin;

};

#endif 
