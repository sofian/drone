
#include "Gear_Osc.h"
#include "Engine.h"
#include "GearMaker.h"
#include <iostream>
#include <math.h>

Register_Gear(MAKERGear_Osc, Gear_Osc, "Oscillator")

Gear_Osc::Gear_Osc(Engine *engine, std::string name) : Gear(engine, "Oscillator", name)
{
  addPlug(_PARAM_FREQ = new PlugIn<SignalType>(this, "Freq", new SignalType(440.0f)));
  addPlug(_PARAM_AMP = new PlugIn<SignalType>(this, "Amp", new SignalType(1.0f)));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "Out"));
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
  MatrixType<float> bufferfreq  = _PARAM_FREQ->type()->buffer();
  MatrixType<float> bufferamp  = _PARAM_AMP->type()->buffer();
  MatrixType<float> bufferout = _AUDIO_OUT->type()->buffer();

  int signal_blocksize = Engine::signalInfo().blockSize();

  _currentTime = _engine->currentTime();


  for (int i=0;i<signal_blocksize;i++)
  {
    bufferout[i] = bufferamp[i] * cos(bufferfreq[i] * _currentTime * 6.28318f);
    _currentTime+=Engine::signalInfo().timePerSample();
    //std::cout << bufferout[i] << " : " << time << std::endl;
  }

}
