#ifndef GEAR_VIDEOADD_INCLUDED
#define GEAR_VIDEOADD_INCLUDED


#include "Gear.h"


class Gear_VideoAdd : public Gear
{
public:

    Gear_VideoAdd(Engine *engine, std::string name);
    virtual ~Gear_VideoAdd();
    
    void runVideo();

    PlugVideoIn* VIDEO_IN_A(){return _VIDEO_IN_A;};
    PlugVideoIn* VIDEO_IN_B(){return _VIDEO_IN_B;};
    PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
    
    bool ready();

private:

	PlugVideoIn *_VIDEO_IN_A;
    PlugVideoIn *_VIDEO_IN_B;
	PlugVideoOut *_VIDEO_OUT;

    //local var
    Video_T *_imageA; 
    Video_T *_imageB; 
    Video_T *_outImage; 
    RGBA *_dataA;
    RGBA *_dataB;
    RGBA *_outData;
    RGBA *_largerImage;
    RGBA *_tallerImage;

    int _iterSizeY;
    int _iterSizeX;    
    int _imageASizeX;
    int _imageASizeY;
    int _imageBSizeX;
    int _imageBSizeY;
    int _imageOutSizeX;
    int _imageOutSizeY;
        
    double *_mmxImageInA;
    double *_mmxImageInB;
    double *_mmxImageOut;

    int _remainIterSizeY;
    int _remainIterSizeX;
    
};

#endif
