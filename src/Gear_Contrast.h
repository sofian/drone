#ifndef GEAR_CONTRAST_INCLUDED
#define GEAR_CONTRAST_INCLUDED


#include "Gear.h"


class Gear_Contrast : public Gear
{
public:

    Gear_Contrast(Engine *engine, std::string name);
    virtual ~Gear_Contrast();
    
    void runVideo();

    PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
    PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
    
    PlugSignalIn* CONTRAST_IN(){return _CONTRAST_IN;};
    
    bool ready();

private:


    PlugVideoIn *_VIDEO_IN;
	PlugVideoOut *_VIDEO_OUT;
    PlugSignalIn *_CONTRAST_IN;

    //local var
    Video_T *_image; 
    Video_T *_outImage; 
    RGBA *_data;
    RGBA *_outData;

    int _iterSizeY;
    int _iterSizeX;    
            
    short _r;
    short _g;
    short _b;

    unsigned char *_imageIn;
    unsigned char *_imageOut;
        
};

#endif
