// Gear_Gain.cpp: implementation of the Gear_Gain class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_Gain.h"
#include "Engine.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_Gain, Gear_Gain, "Gain")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_Gain::Gear_Gain(Engine *engine, std::string name) : Gear(engine, "Gain", name)
{

  _AUDIO_IN = addPlugSignalIn("In", 0.0f);
  _PARAM_GAIN = addPlugSignalIn("Gain", 1.0f);
  _AUDIO_OUT = addPlugSignalOut("Out");
}

Gear_Gain::~Gear_Gain()
{

}

bool Gear_Gain::ready()
{
  return(_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

void Gear_Gain::runAudio()
{
  Signal_T *bufferin = _AUDIO_IN->buffer();
  Signal_T *bufferout = _AUDIO_OUT->buffer();
  Signal_T *buffergain  = _PARAM_GAIN->buffer();


  int signal_blocksize = Engine::signalInfo().blockSize();
  for (int i=0;i<signal_blocksize;i++)
  {
    //cout << bufferin[i] << endl;
    bufferout[i]=bufferin[i] * buffergain[i];                
  }

}
