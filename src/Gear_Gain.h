#ifndef GEAR_GAIN_INCLUDED
#define GEAR_GAIN_INCLUDED

#include "Gear.h"

class PlugSignal;

class Gear_Gain : public Gear  
{
public:
	Gear_Gain(Engine *engine, std::string name);
	virtual ~Gear_Gain();

    void runAudio();
	
    PlugSignalIn* AUDIO_IN(){return _AUDIO_IN;};
    PlugSignalIn* PARAM_GAIN(){return _PARAM_GAIN;};
	PlugSignalOut* AUDIO_OUT(){return _AUDIO_OUT;};

    bool ready();

private:
    PlugSignalIn *_AUDIO_IN, *_PARAM_GAIN;
	PlugSignalOut *_AUDIO_OUT;

};

#endif 
