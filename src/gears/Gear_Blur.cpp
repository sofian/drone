#include "Gear_Blur.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_Blur, Gear_Blur, "Blur")

Gear_Blur::Gear_Blur(Engine *engine, std::string name) : Gear(engine, "Blur", name), _amountMapData(0)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_AMOUNT_MAP_IN = new PlugIn<VideoTypeRGBA>(this, "Amount Map"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(0.5f)));
  _table = new SummedAreaTable();
}

Gear_Blur::~Gear_Blur()
{
  delete _table;
}

void Gear_Blur::init()
{
  _table->setImage(_VIDEO_IN->type()->image());
}

bool Gear_Blur::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Blur::runVideo()
{
  _image = _VIDEO_IN->type()->image();

  _outImage = _VIDEO_OUT->type()->image();
  _outImage->resize(_image->width(), _image->height());
  _sizeY = _image->sizeY();
  _sizeX = _image->sizeX();

  ////////////////////////////

  _blurSize=(int) _AMOUNT_IN->type()->value();

  if (_blurSize <= 0)
    memcpy(_outImage->data(), _image->data(), _image->size()*sizeof(RGBA));
  else
  {
    _table->setImage(_image);
    _table->buildTable();

    _data = (unsigned char*)_image->data();    
    _outData = (unsigned char*)_outImage->data();

    for (int y=0;y<_sizeY;y++)
    {
      for (int x=0;x<_sizeX;x++)
      {
        _x1 = x - _blurSize - 1;
        _x2 = x + _blurSize;
        _y1 = y - _blurSize - 1;
        _y2 = y + _blurSize;
        
        _x2 = MIN(_x2, _sizeX-1);
        _y2 = MIN(_y2, _sizeY-1);
        
        _table->getSum(&_sum, _x1, _y1, _x2, _y2);
        
        divide((int*)&_sum, _table->getArea(_x1, _y1, _x2, _y2), SIZE_RGBA);
        copy(_outData, (int*)&_sum, SIZE_RGBA);
        _outData+=SIZE_RGBA;
      }
    }                                   
  }

}
