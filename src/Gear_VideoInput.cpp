#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/types.h>

#include <string>
#include <iostream>

#include "Gear_VideoInput.h"
#include "Engine.h"

#include "GearMaker.h"



Register_Gear(MAKERGear_VideoInput, Gear_VideoInput, "VideoInput");

const std::string Gear_VideoInput::SETTING_DEVICE = "Device";
const std::string Gear_VideoInput::DEFAULT_DEVICE = "/dev/video0";

Gear_VideoInput::Gear_VideoInput(Engine *engine, std::string name) : Gear(engine, "VideoInput", name),
    _sizeX(0),
    _sizeY(0),
    _device(0),
    _bufferBGRA(NULL)

{    
    _VIDEO_OUT = addPlugVideoOut("Out");       
    
    _settings.add(Property::STRING, SETTING_DEVICE)->valueStr(DEFAULT_DEVICE);    

    resetInputDevice();
}

Gear_VideoInput::~Gear_VideoInput()
{
}
    
bool Gear_VideoInput::ready()
{
    return (_VIDEO_OUT->connected());
}

void Gear_VideoInput::init()
{
    initInputDevice();
}

void Gear_VideoInput::onUpdateSettings()
{
    initInputDevice();
}

void Gear_VideoInput::resetInputDevice()
{
    if (_device!=0)
        close(_device);
    
    _sizeX=0;
    _sizeY=0;

    if (_bufferBGRA!=NULL)
    {
        free(_bufferBGRA);
        _bufferBGRA=NULL;
    }

    memset(&_vidCap, 0, sizeof(video_capability));
    memset(&_vidWin, 0, sizeof(video_window));
    memset(&_vidPic, 0, sizeof(video_picture));    
}

void Gear_VideoInput::initInputDevice()
{
    resetInputDevice();

    _device = open(_settings.get(SETTING_DEVICE)->valueStr().c_str(), O_RDWR | O_NONBLOCK);

    if (_device<=0)
    {
        std::cout << "fail to open device " << _settings.get(SETTING_DEVICE)->valueStr().c_str() << std::endl;
        return;
    }

    //get info
    ioctl(_device, VIDIOCGCAP, &_vidCap);
    ioctl(_device, VIDIOCGWIN, &_vidWin);
    ioctl(_device, VIDIOCGPICT, &_vidPic);
    
    std::cout << "palette : " << _vidPic.palette << std::endl;

    _vidPic.palette = VIDEO_PALETTE_RGB32;    
    ioctl(_device, VIDIOCSPICT, &_vidPic);

    
    _vidWin.clips = _vidClips;
    _vidWin.clipcount = 0;
    _sizeX = _vidCap.maxwidth;
    _sizeY = _vidCap.maxheight;

    //show info
    std::cout << "-- device info --" << std::endl;
    std::cout << "name   : " << _vidCap.name << std::endl;
    std::cout << "type   : " << _vidCap.type << std::endl;
    std::cout << "size X : " << _sizeX << std::endl;
    std::cout << "size Y : " << _sizeY << std::endl;
    std::cout << "bpp    : " << _vidPic.depth << std::endl;

    //prepare and allocate mmap
/*     ioctl(_device, VIDIOCGMBUF, &_vidMBuf);                                                       */
/*                                                                                                   */
/*     _bufferRGB24 = (unsigned char*) mmap(NULL, _vidMBuf.size, PROT_READ, MAP_SHARED, _device, 0); */
/*                                                                                                   */
/*     _vidMMap.format = VIDEO_PALETTE_RGB24;                                                        */
/*     _vidMMap.frame  = 0;                                                                          */
/*     _vidMMap.width  = _sizeX;                                                                     */
/*     _vidMMap.height = _sizeY;                                                                     */


    _bufferBGRA = (unsigned char*)malloc(_sizeX * _sizeY * 4);
    _VIDEO_OUT->canvas()->allocate(_sizeX, _sizeY);
}

void Gear_VideoInput::runVideo()
{
    _outData = (unsigned char*) _VIDEO_OUT->canvas()->_data;
                   
    int len=0;
          
    len = read(_device, _bufferBGRA, _sizeX * _sizeY * 4);
    
    //convert BGRA -> RGBA
    //need optimisation
    if (len)
    {                
        int imgSize=_sizeX*_sizeY;
        _tempOutData = _outData;
        _tempInData = _bufferBGRA;
        for (int i=0;i<imgSize;i++)
        {
            *(_tempOutData) = *(_tempInData+2);
            *(_tempOutData+1) = *(_tempInData+1);
            *(_tempOutData+2) = *(_tempInData);

            _tempOutData+=4;
            _tempInData+=4;
        }

    }
     

}



