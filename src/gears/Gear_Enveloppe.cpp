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
    
    _AUDIO_IN = addPlugSignalIn("In", 0.0f);
    _PARAM_RELEASE = addPlugSignalIn("Rel", .1f);
    _PARAM_ATTACK = addPlugSignalIn("Att", .1f);

    _AUDIO_OUT = addPlugSignalOut("Out");
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
    Signal_T *bufferin = _AUDIO_IN->buffer();
    Signal_T *bufferout = _AUDIO_OUT->buffer();
    Signal_T *bufferrel  = _PARAM_RELEASE->buffer();
    Signal_T *bufferatt  = _PARAM_ATTACK->buffer();
    
    int samplerate = Engine::signalInfo().sampleRate();
    int signal_blocksize = Engine::signalInfo().blockSize();
    
    float ga = (float) exp(-1/(samplerate* *bufferatt));
    float gr = (float) exp(-1/(samplerate* *bufferrel));
 
    for(int i=0;i<signal_blocksize;i++)
    {
      _envin = abs(*bufferin++);
      
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
