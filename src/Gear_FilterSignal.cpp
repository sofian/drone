// Gear_FilterSignal.cpp: implementation of the Gear_FilterSignal class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_FilterSignal.h"
#include "Engine.h"

#include "GearMaker.h"

#include <math.h>

Register_Gear(MAKERGear_FilterSignal, Gear_FilterSignal, "FilterSignal")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_FilterSignal::Gear_FilterSignal(Engine *engine, std::string name) : Gear(engine, "FilterSignal", name),
    _convolutionShape(NULL)
{    
    _AUDIO_IN = addPlugSignalIn("In", 0.0f);
    _AUDIO_OUT = addPlugSignalOut("Out");

    int blockSize = Engine::signalInfo().blockSize();
    _convolutionShape = new Signal_T[blockSize];

    float f=440.0f;
    float wc=2.0 * 6.28318f * f * blockSize/44100.0f;

    for (int i=0;i<blockSize;i++)
        _convolutionShape[i] = sin(wc*((float)i - ((float)blockSize/2.0f))) / (3.14f * ((float)i - ((float)blockSize/2.0f)));
    
}

Gear_FilterSignal::~Gear_FilterSignal()
{

}

bool Gear_FilterSignal::ready()
{
    return (_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

void Gear_FilterSignal::init()
{    
    if (_freqAnalyzer!=NULL)
        delete _freqAnalyzer;
    
    _freqAnalyzer = new FrequencyAnalyser(Engine::signalInfo().blockSize());    
}


void Gear_FilterSignal::runAudio()
{
    Signal_T *bufferin = _AUDIO_IN->buffer();
    Signal_T *bufferout = _AUDIO_OUT->buffer();

    Signal_T kernel[Engine::signalInfo().blockSize()];

    for (int i=0;i<Engine::signalInfo().blockSize()/2;++i)
    {
        kernel[i]=1.0f;
    }

    for (int i=Engine::signalInfo().blockSize()/2;i<Engine::signalInfo().blockSize();++i)
    {
        kernel[i]=0.0f;
    }


    //_freqAnalyzer->performAnalysis(bufferin);
    //_freqAnalyzer->backward(bufferout);

    _freqAnalyzer->filter(bufferin, bufferout);
    //_freqAnalyzer->frequencyToTime(kernel, bufferout);

}
