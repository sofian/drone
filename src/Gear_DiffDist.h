#ifndef GEAR_DIFFDIST_INCLUDED
#define GEAR_DIFFDIST_INCLUDED


#include "Gear.h"


class Gear_DiffDist : public Gear
{
public:

    Gear_DiffDist(Engine *engine, std::string name);
    virtual ~Gear_DiffDist();
    
    void runVideo();

    PlugSignalIn* FACTOR_IN(){return _FACTOR_IN;};    
    PlugVideoIn* VIDEO_IN_A(){return _VIDEO_IN_A;};
    PlugVideoIn* VIDEO_IN_B(){return _VIDEO_IN_B;};
    PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
    
    bool ready();

private:

	PlugSignalIn *_FACTOR_IN;
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

    int _iterSizeY;
    int _iterSizeX;    
    int _imageASizeX;
    int _imageASizeY;
    int _imageBSizeX;
    int _imageBSizeY;
    int _imageOutSizeX;
    int _imageOutSizeY;
        
    
};

#endif
