#include "Gear_VideoOutput.h"

#include "GearMaker.h"
#include "Engine.h"
#include "VideoOutput.h"
#include "VideoOutputMaker.h"

#include <iostream>

Register_Gear(MAKERGear_VideoOutput, Gear_VideoOutput, "VideoOutput")

const int Gear_VideoOutput::DEFAULT_XRES = 352;
const int Gear_VideoOutput::DEFAULT_YRES = 240;
const int Gear_VideoOutput::DEFAULT_BPP = 32; 
const bool Gear_VideoOutput::DEFAULT_FULLSCREEN = false; 

const std::string Gear_VideoOutput::SETTING_XRES = "XRes";
const std::string Gear_VideoOutput::SETTING_YRES = "YRes";
const std::string Gear_VideoOutput::SETTING_BPP = "BPP";
const std::string Gear_VideoOutput::SETTING_FULLSCREEN = "FullScreen";


Gear_VideoOutput::Gear_VideoOutput(Engine *engine, std::string name) : 
    Gear(engine, "VideoOutput", name),
    _videoOutput(NULL)
{
   
    _VIDEO_IN = addPlugVideoIn(name);

    _settings.add(Property::INT, SETTING_XRES)->valueInt(DEFAULT_XRES);
    _settings.add(Property::INT, SETTING_YRES)->valueInt(DEFAULT_YRES);
    _settings.add(Property::INT, SETTING_BPP)->valueInt(DEFAULT_BPP);
    _settings.add(Property::BOOL, SETTING_FULLSCREEN)->valueBool(DEFAULT_FULLSCREEN);

}

Gear_VideoOutput::~Gear_VideoOutput()
{    
    delete _videoOutput;
}


bool Gear_VideoOutput::ready()
{
    return _VIDEO_IN->connected();
}

void Gear_VideoOutput::init()
{    	
    _videoOutput = VideoOutputMaker::makeVideoOutput("Gl");
    
    if (_videoOutput==NULL)
        return;
    
    if (!_videoOutput->init(_settings.get(SETTING_XRES)->valueInt(), _settings.get(SETTING_YRES)->valueInt(),
                            _settings.get(SETTING_BPP)->valueInt(), false))
    {
        delete _videoOutput;
        _videoOutput=NULL;
    }
}

void Gear_VideoOutput::prePlay()
{

}

void Gear_VideoOutput::postPlay()
{
}


void Gear_VideoOutput::runVideo()
{       
    if (!_VIDEO_IN->connected() || _videoOutput==NULL)
        return;
                
    if (_videoOutput==NULL)
        return;
        
    _videoOutput->render(*(_VIDEO_IN->canvas()));
}






