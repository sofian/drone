#include <iostream>
#include "Gear_AudioSource.h"
#include "Engine.h"

#include "GearMaker.h"


Register_Gear(MAKERGear_AudioSource, Gear_AudioSource, "AudioSource");

const std::string Gear_AudioSource::SETTING_FILENAME = "Filename";

Gear_AudioSource::Gear_AudioSource(Engine *engine, std::string name) : Gear(engine, "AudioSource", name),
    _File(NULL)
{        
    _AUDIO_OUT = addPlugSignalOut("Out");       
    
    _settings.add(Property::FILENAME, SETTING_FILENAME,"");    
}

Gear_AudioSource::~Gear_AudioSource()
{
    if (_File!=NULL)
        sf_close(_File);
    
}
    
bool Gear_AudioSource::ready()
{
    return (_File!=NULL && _AUDIO_OUT->connected());
}

void Gear_AudioSource::onUpdateSettings()
{

    if (_File!=NULL)
        sf_close(_File);

    _File = sf_open(_settings.get(SETTING_FILENAME).c_str(), SFM_READ, &_SFInfo);    

    if (_File==NULL)
    {
        std::cout << "error opening soundfile!" << std::endl;
        return;
    }
}


void Gear_AudioSource::runAudio()
{
    _audioBuffer = _AUDIO_OUT->buffer();
    
    if ( sf_readf_float(_File, _audioBuffer, Engine::signalInfo().blockSize()) == 0)
    {
        //go back to the beginning, loop
        sf_seek(_File, 0, SEEK_SET);
    }
}


