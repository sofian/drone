#ifndef GEAR_GRAYSCALE_INCLUDED
#define GEAR_GRAYSCALE_INCLUDED


#include "Gear.h"


class Gear_GrayScale : public Gear
{
public:

    Gear_GrayScale(Engine *engine, std::string name);
    virtual ~Gear_GrayScale();
    
    void runVideo();

    PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
    PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
        
    bool ready();

private:

	PlugVideoIn *_VIDEO_IN;
	PlugVideoOut *_VIDEO_OUT;

    //local var
    Video_T *_image; 
    Video_T *_outImage; 
    RGBA *_data;
    RGBA *_outData;

  int _size;
            
    unsigned int *_imageOut;
    unsigned char *_imageIn;
    
    
        
};

#endif
