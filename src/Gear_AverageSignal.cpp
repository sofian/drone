// Gear_AverageSignal.cpp: implementation of the Gear_AverageSignal class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_AverageSignal.h"
#include "Engine.h"
#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_AverageSignal, Gear_AverageSignal, "AverageSignal")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_AverageSignal::Gear_AverageSignal(Engine *engine, std::string name) : Gear(engine, "AverageSignal", name), 
    _totalSignal(0.0f), _nbSamples(0), _average(0.0f)
{
    
    _AUDIO_IN = addPlugSignalIn("Input", 0.0f);
    _AUDIO_OUT = addPlugSignalOut("Output");
}

Gear_AverageSignal::~Gear_AverageSignal()
{

}

bool Gear_AverageSignal::ready()
{
    return (_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

void Gear_AverageSignal::runAudio()
{
    Signal_T *bufferin = _AUDIO_IN->buffer();
    Signal_T *bufferout = _AUDIO_OUT->buffer();
    
        
	int signal_blocksize = Engine::signalInfo().blockSize();
    for(int i=0;i<signal_blocksize;i++)
    {
        _totalSignal += bufferin[i];
    }
    
    _nbSamples+=signal_blocksize;
    _average = _totalSignal/_nbSamples;

    for(int i=0;i<signal_blocksize;i++)
    {
        bufferout[i] = _average;
    }

    std::cout << _average << std::endl;
    
}
