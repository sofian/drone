#ifndef GEAR_VIDEOINPUT_INCLUDED
#define GEAR_VIDEOINPUT_INCLUDED


#include "Gear.h"

#include <linux/videodev.h>

class Gear_VideoInput : public Gear
{
public:
    static const std::string SETTING_DEVICE;
    static const std::string DEFAULT_DEVICE;

    Gear_VideoInput(Engine *engine, std::string name);
    virtual ~Gear_VideoInput();
        
    void runVideo();
    void init();
    
    PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};

    bool ready();

protected:
    void onUpdateSettings();

private:

    void initInputDevice();
    void resetInputDevice();

    PlugVideoOut *_VIDEO_OUT;

    int _sizeX, _sizeY;
    
    //v4l
    int _device;
    struct video_capability _vidCap;
    struct video_window _vidWin;
    struct video_picture _vidPic;
    struct video_clip _vidClips[32];

    //mmap
    struct video_mbuf _vidMBuf;
    struct video_mmap _vidMMap;    
    unsigned char *_bufferBGRA;

    
    //locals
    unsigned char *_outData;
    unsigned char *_tempOutData;
    unsigned char *_tempInData;
    

    //mmx
    double *_mmxImageIn;
    double *_mmxImageOut;

};

#endif
