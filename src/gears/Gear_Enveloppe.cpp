// Gear_Gain.cpp: implementation of the Gear_Gain class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_Enveloppe.h"
#include "Engine.h"
#include <cmath>
#include "GearMaker.h"

Register_Gear(MAKERGear_Enveloppe, Gear_Enveloppe, "Enveloppe")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_Enveloppe::Gear_Enveloppe(Engine *engine, std::string name) : Gear(engine, "Enveloppe", name)
{    
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "In"));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "Out"));

  addPlug(_PARAM_RELEASE = new PlugIn<ValueType>(this, "Rel", new ValueType(.2f,0,3)));
  addPlug(_PARAM_ATTACK = new PlugIn<ValueType>(this, "Att", new ValueType(.2f,0,3)));
}

Gear_Enveloppe::~Gear_Enveloppe()
{

}

void Gear_Enveloppe::prePlay()
{
  _lastenvel=0;

}    

bool Gear_Enveloppe::ready()
{
    return (_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

void Gear_Enveloppe::runAudio()
{
  const float *bufferin = _AUDIO_IN->type()->data();
  float *bufferout = _AUDIO_OUT->type()->data();
  float release = _PARAM_RELEASE->type()->value();
  float attack = _PARAM_ATTACK->type()->value();
  
  int samplerate = Engine::signalInfo().sampleRate();
  int signal_blocksize = Engine::signalInfo().blockSize();
  
  float ga = (float) exp(-1/(samplerate* attack));
  float gr = (float) exp(-1/(samplerate* release));
  
  for(int i=0;i<signal_blocksize;i++)
  {
    _envin = fabs(*bufferin++);
    
    if(_envel < _envin)
    {
      _envel *= ga;
      _envel += (1-ga)*_envin;
    }
    else
    {
      _envel *= gr;
      _envel += (1-gr)*_envin;
    }
    
    _lastenvel = bufferout[i] = _envel;
  }      
}
