#ifndef GEAR_NGONE_INCLUDED
#define GEAR_NGONE_INCLUDED


#include "Gear.h"


class Gear_NGone : public Gear
{
public:

    Gear_NGone(Engine *engine, std::string name);
    virtual ~Gear_NGone();
    
    void runVideo();

    PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
    PlugVideoComposeIn* VCOMPOSE_IN(){return _VCOMPOSE_IN;};
    PlugVideoComposeOut* VCOMPOSE_OUT(){return _VCOMPOSE_OUT;};
    PlugSignalIn *SIGNAL_X_IN(){return _SIGNAL_X_IN;};
    PlugSignalIn *SIGNAL_Y_IN(){return _SIGNAL_Y_IN;};
              
    bool ready();

private:

	PlugVideoIn *_VIDEO_IN;
    PlugVideoComposeOut *_VCOMPOSE_OUT;
    PlugVideoComposeIn *_VCOMPOSE_IN;
    PlugSignalIn *_SIGNAL_X_IN;
    PlugSignalIn *_SIGNAL_Y_IN;
    
    //locals
    Video_T *_image;
        
    float _sizeX;
    float _sizeY;

    float _texSizeX;
    float _texSizeY;
    
    Signal_T *_signalBufferX;
    Signal_T *_signalBufferY;
    
};

#endif
