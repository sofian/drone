// Gear_DataOut.cpp: implementation of the Gear_DataOut class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_DataOut.h"
#include "Engine.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_DataOut, Gear_DataOut, "DataOut")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_DataOut::Gear_DataOut(Engine *engine, std::string name) : Gear(engine, "DataOut", name)
{

  _FileOut.open("dataout.txt");
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "Input", new SignalType(0.0f)));
}

Gear_DataOut::~Gear_DataOut()
{
  _FileOut.close();
}

bool Gear_DataOut::ready()
{
  return(_AUDIO_IN->connected());
}

void Gear_DataOut::runAudio()
{
  const float *bufferin = _AUDIO_IN->type()->data();

  int signal_blocksize = Engine::signalInfo().blockSize();
  for (int i=0;i<signal_blocksize;i++)
    _FileOut << bufferin[i] << std::endl;

}
