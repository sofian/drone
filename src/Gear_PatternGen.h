#ifndef GEAR_PATTERNGEN_INCLUDED
#define GEAR_PATTERNGEN_INCLUDED


#include "Gear.h"
#include <string>

class Gear_PatternGen : public Gear
{
public:

    static const std::string SETTING_SIZE_X;
    static const std::string SETTING_SIZE_Y;

    Gear_PatternGen(Engine *engine, std::string name);
    virtual ~Gear_PatternGen();
        
    void runVideo();
    
    PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
    
    PlugSignalIn* PARAM_IN(){return _PARAM_IN;};
    
    bool ready();

protected:
    void onUpdateSettings();

private:

	PlugVideoOut *_VIDEO_OUT;
    PlugSignalIn *_PARAM_IN;

    Video_T *_outImage; 
    RGBA *_outData;

    int _iterSizeY;
    int _iterSizeX;    
                
    unsigned char *_imageOut;
        
};

#endif
