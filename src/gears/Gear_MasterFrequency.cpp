// Gear_MasterFrequency.cpp: implementation of the Gear_MasterFrequency class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_MasterFrequency.h"
#include "Engine.h"
#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_MasterFrequency, Gear_MasterFrequency, "MasterFrequency")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_MasterFrequency::Gear_MasterFrequency(Engine *engine, std::string name) : Gear(engine, "MasterFrequency", name),
_freqAnalyser(NULL)
{

  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "In", new SignalType(0.0f)));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "Out"));
}

Gear_MasterFrequency::~Gear_MasterFrequency() 
{

}

bool Gear_MasterFrequency::ready()
{
  return(_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

void Gear_MasterFrequency::init()
{
  if (_freqAnalyser!=NULL)
    delete _freqAnalyser;

  _freqAnalyser = new FrequencyAnalyser(Engine::signalInfo().blockSize());    
}

void Gear_MasterFrequency::runAudio()
{
  //   MatrixType<float> bufferin = _AUDIO_IN->type()->buffer();
  //   MatrixType<float> bufferout = _AUDIO_OUT->type()->buffer();

  //_freqAnalyser->performAnalysis(bufferin);

  //float masterFrequency = _freqAnalyser->getMasterFrequency();

  //std::cout << masterFrequency << std::endl;

/*     if (masterFrequency < 100) */
/*     {                          */
/*         masterFrequency=0;     */
/*     }                          */

  int signal_blocksize = Engine::signalInfo().blockSize();
  for (int i=0;i<signal_blocksize;i++)
  {
    //  bufferout[i]=masterFrequency;
  }

}
