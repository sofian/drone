#include "Gear_Blur.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_Blur, Gear_Blur, "Blur")

Gear_Blur::Gear_Blur(Engine *engine, std::string name) : Gear(engine, "Blur", name), _amountMapData(0)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _AMOUNT_MAP_IN = addPlugVideoIn("Amount Map");

  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _AMOUNT_IN = addPlugSignalIn("Amount", 0.5f);
  _table = new SummedAreaTable();
}

Gear_Blur::~Gear_Blur()
{
  delete _table;
}

void Gear_Blur::init()
{
  _table->setCanvas(_VIDEO_IN->canvas());
}

bool Gear_Blur::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Blur::runVideo()
{
  _image = _VIDEO_IN->canvas();

  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());
  _sizeY = _image->sizeY();
  _sizeX = _image->sizeX();

  ////////////////////////////

  _blurSize=(int) (_AMOUNT_IN->buffer()[0]);

  if (_blurSize <= 0)
    memcpy(_outImage->_data, _image->_data, _sizeX*_sizeY*sizeof(RGBA));
  else
  {
    _table->setCanvas(_image);
    _table->buildTable();

    _data = (unsigned char*)_image->_data;    
    _outData = (unsigned char*)_outImage->_data;

    for (int y=0;y<_sizeY;y++)
    {
      for (int x=0;x<_sizeX;x++)
      {
        _x1 = x - _blurSize - 1;
        _x2 = x + _blurSize;
        _y1 = y - _blurSize - 1;
        _y2 = y + _blurSize;

        if (_x2 >= _sizeX)_x2 = _sizeX-1;
        if (_y2 >= _sizeY)_y2 = _sizeY-1;

        _table->getSum(&_sum, _x1, _y1, _x2, _y2);

        divide((int*)&_sum, _table->getArea(_x1, _y1, _x2, _y2), SIZE_RGBA);
        copy(_outData, (int*)&_sum, SIZE_RGBA);
        _outData+=SIZE_RGBA;
      }
    }                                    
  }

}
