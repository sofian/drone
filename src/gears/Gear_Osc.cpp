
#include "Gear_Osc.h"
#include "Engine.h"
#include "GearMaker.h"
#include <iostream>
#include <math.h>

Register_Gear(MAKERGear_Osc, Gear_Osc, "Oscillator")

Gear_Osc::Gear_Osc(Engine *engine, std::string name) : Gear(engine, "Oscillator", name)
{
  _PARAM_FREQ = addPlugSignalIn("Freq", 440.0f);
  _PARAM_AMP = addPlugSignalIn("Amp", 1.0f);
  _AUDIO_OUT = addPlugSignalOut("Out");
}

Gear_Osc::~Gear_Osc()
{

}

bool Gear_Osc::ready()
{
  return _AUDIO_OUT->connected();
}

void Gear_Osc::runAudio()
{        
  Signal_T *bufferfreq  = _PARAM_FREQ->buffer();
  Signal_T *bufferamp  = _PARAM_AMP->buffer();
  Signal_T *bufferout = _AUDIO_OUT->buffer();

  int signal_blocksize = Engine::signalInfo().blockSize();

  _currentTime = _engine->currentTime();


  for (int i=0;i<signal_blocksize;i++)
  {
    bufferout[i] = bufferamp[i] * cos(bufferfreq[i] * _currentTime * 6.28318f);
    _currentTime+=Engine::signalInfo().timePerSample();
    //std::cout << bufferout[i] << " : " << time << std::endl;
  }

}
