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
                                                                         _convolutionShape()
{    
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "In", new SignalType(0.0f)));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "Out"));

  int blockSize = Engine::signalInfo().blockSize();
  _convolutionShape.resize(blockSize);

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
  return(_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

void Gear_FilterSignal::init()
{    
  if (_freqAnalyzer!=NULL)
    delete _freqAnalyzer;

  _freqAnalyzer = new FrequencyAnalyser(Engine::signalInfo().blockSize());    
}


void Gear_FilterSignal::runAudio()
{
  SignalType bufferin = _AUDIO_IN->type();
  SignalType bufferout = _AUDIO_OUT->type();

  float kernel[Engine::signalInfo().blockSize()];

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

  _freqAnalyzer->filter(bufferin.data(), bufferout.data());
  //_freqAnalyzer->frequencyToTime(kernel, bufferout);

}
