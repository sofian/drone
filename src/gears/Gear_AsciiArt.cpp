#include "Gear_AsciiArt.h"                       
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_AsciiArt, Gear_AsciiArt, "AsciiArt")

Gear_AsciiArt::Gear_AsciiArt(Engine *engine, std::string name) : Gear(engine, "AsciiArt", name)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _AMOUNT_IN = addPlugSignalIn("Amount", 1.0f);
}

Gear_AsciiArt::~Gear_AsciiArt()
{

}

bool Gear_AsciiArt::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}



void Gear_AsciiArt::runVideo()
{
  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());
  _data = (unsigned char*)_image->_data;
  _outData = (unsigned char*)_outImage->_data;
  float tresh = _AMOUNT_IN->buffer()[0];

  _sizeX = _image->sizeX();
  _sizeY = _image->sizeY();

  int pp=0;
  for (int y=1;y<_sizeY-2;y++)
  {
    for (int x=1;x<_sizeX-2;x++)
    {
      for (int z=0;z<4;z++)
      {
        _outData[y*_sizeX*4+x*4+z] = (4* _data[y*_sizeX*4+x*4+z] - 
                                      _data[(y-1)*_sizeX*4+x*4+z] -
                                      _data[(y+1)*_sizeX*4+x*4+z] -
                                      _data[y*_sizeX*4+(x-1)*4+z] -
                                      _data[y*_sizeX*4+(x+1)*4+z])/2;

        if (_outData[y*_sizeX*4+x*4+z]<tresh)
          _outData[y*_sizeX*4+x*4+z]=0;

      }
    }
  }
}
