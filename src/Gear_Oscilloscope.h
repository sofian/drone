#ifndef GEAR_OSCILLOSCOPE_INCLUDED
#define GEAR_OSCILLOSCOPE_INCLUDED


#include "Gear.h"

class PlugVideo;
class PlugSignal;

class Gear_Oscilloscope : public Gear
{
public:

    Gear_Oscilloscope(Engine *engine, std::string name);
    virtual ~Gear_Oscilloscope();
    
    void runVideo();

    PlugSignalIn* AUDIO_IN(){return _AUDIO_IN;};
    PlugVideoComposeIn* VCOMPOSE_IN(){return _VCOMPOSE_IN;};    
    PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};

    bool ready();

private:

	PlugSignalIn *_AUDIO_IN;
    PlugVideoOut *_VIDEO_OUT;
    PlugVideoComposeIn *_VCOMPOSE_IN;
    
    
    Video_T *_outImage; 
    RGBA *_outData;
    
    
};

#endif
