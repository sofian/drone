
#ifndef GEAR_VideoSource_INCLUDED
#define GEAR_VideoSource_INCLUDED


#include "Gear.h"

#include "libmpeg3.h"

class PlugVideo;
class PlugSignal;

class Gear_VideoSource : public Gear
{
public:
    static const std::string SETTING_FILENAME;

    Gear_VideoSource(Engine *engine, std::string name);
    virtual ~Gear_VideoSource();
        
    void runVideo();
    void runAudio();
    
    PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
    PlugSignalOut* AUDIO_OUT(){return _AUDIO_OUT;};

    bool ready();

protected:
    void onUpdateSettings();

private:

    PlugVideoOut *_VIDEO_OUT;
    PlugSignalOut *_AUDIO_OUT;
    mpeg3_t* _File;

    int _SizeX, _SizeY;
    
    //locals
    Video_T *_image;
    Signal_T *_audioBuffer;
    RGBA *_Frame[1024];

    //mmx
    double *_mmxImageIn;
    double *_mmxImageOut;

};

#endif
