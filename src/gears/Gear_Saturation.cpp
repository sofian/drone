#include "Gear_Saturation.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"



Register_Gear(MAKERGear_Saturation, Gear_Saturation, "Saturation")

Gear_Saturation::Gear_Saturation(Engine *engine, std::string name) : Gear(engine, "Saturation", name)
{
    addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
    addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
    addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(1.0f)));

}

Gear_Saturation::~Gear_Saturation()
{

}

bool Gear_Saturation::ready()
{
    return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Saturation::runVideo()
{
    _image = _VIDEO_IN->type();
    _outImage = _VIDEO_OUT->type();
    _outImage->resize(_image->width(), _image->height());
    _data = _image->data();    
    _outData = _outImage->data();

    _iterSizeX = _image->width();
    _iterSizeY = _image->height();
    
    float amount = _AMOUNT_IN->type()->value();
    int bw=0;
  
        
    for (int y=0;y<_iterSizeY;y++)
    {
        _imageIn = (unsigned char*)&_data[y*_iterSizeX];
        _imageOut = (unsigned char*)&_outData[y*_iterSizeX];

        for (int x=0;x<_iterSizeX;x++)
        {
            _r = *(_imageIn);
            _g = *(_imageIn+1);
            _b = *(_imageIn+2);

            bw = (_r+_g+_b) / 3;

            _r = (short) (bw + (_r - bw)*amount);
            _g = (short) (bw + (_g - bw)*amount);
            _b = (short) (bw + (_b - bw)*amount);

            
            if (_r>255)
                *_imageOut = 255;
            else
                if (_r<0)
                    *_imageOut = 0;
                else
                    *_imageOut = _r;

            if (_g>255)
                *(_imageOut+1) = 255;
            else
                if (_g<0)
                    *(_imageOut+1) = 0;
                else
                    *(_imageOut+1) = _g;

            if (_b>255)
                *(_imageOut+2) = 255;
            else
                if (_b<0)
                    *(_imageOut+2) = 0;
                else
                    *(_imageOut+2) = _b;
            
           _imageIn+=4;
           _imageOut+=4;
        }
    }
}


