// Gear_AverageSignal.cpp: implementation of the Gear_AverageSignal class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_AverageSignal.h"
#include "Engine.h"
#include <iostream>
#include <math.h>

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
    _cbAudioIn = new CircularBufferSignal(0.0f);
}

Gear_AverageSignal::~Gear_AverageSignal()
{
  delete _cbAudioIn;
}

bool Gear_AverageSignal::ready()
{
    return (_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

void Gear_AverageSignal::runAudio()
{
    Signal_T *bufferin = _AUDIO_IN->buffer();
    Signal_T *bufferout = _AUDIO_OUT->buffer();    
    float sqaverage=0.0f;
      
	int signal_blocksize = Engine::signalInfo().blockSize();
     
    // we assume signal_blocksize < size of circular buffer
    _cbAudioIn->append(bufferin, signal_blocksize);
    
    CIRCBUF_SIGNAL_T_FORBEGIN(_cbAudioIn,-1024,-1)
      sqaverage+=*(cbptr)*(*(cbptr++));
    CIRCBUF_SIGNAL_T_FOREND;

    sqaverage /= signal_blocksize;

    for(int i=0;i<signal_blocksize;i++)
    {
        bufferout[i] = sqrt(sqaverage);
    }

    std::cerr<<sqrt(sqaverage)<<",";
    
}
