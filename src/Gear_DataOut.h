#ifndef GEAR_DATAOUT_INCLUDED
#define GEAR_DATAOUT_INCLUDED

#include <fstream>
#include "Gear.h"

class PlugSignal;

class Gear_DataOut : public Gear  
{
public:
	Gear_DataOut(Engine *engine, std::string name);
	virtual ~Gear_DataOut();

    void runAudio();
   
    PlugSignalIn* AUDIO_IN(){return _AUDIO_IN;};

    bool ready();
    
private:
	std::ofstream _FileOut;
    PlugSignalIn *_AUDIO_IN;

};

#endif 
