#include "Gear_MedianFilter.h"

#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_MedianFilter, Gear_MedianFilter, "MedianFilter")

Gear_MedianFilter::Gear_MedianFilter(Engine *engine, std::string name) : Gear(engine, "MedianFilter", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(1)));
}

Gear_MedianFilter::~Gear_MedianFilter()
{
}

bool Gear_MedianFilter::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_MedianFilter::runVideo()
{
  _image = _VIDEO_IN->type();

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _sizeY = _image->height();
  _sizeX = _image->width();

  _data = (unsigned char*)_image->data();    
  _outData = (unsigned char*)_outImage->data();

  _medianSize = (int) _AMOUNT_IN->type()->value();

  ////////////////////////////

  for (int y=0;y<_sizeY;y++)
  {
    for (int x=0;x<_sizeX;x++)
    {
      _x1 = x - _medianSize;
      _x2 = x + _medianSize;
      _y1 = y - _medianSize;
      _y2 = y + _medianSize;

      if (_x1 < 0)_x1 = 0;
      if (_x2 < 0)_x2 = 0;
      if (_y1 < 0)_y1 = 0;
      if (_y2 < 0)_y2 = 0;

      if (_x1 >= _sizeX)_x1 = _sizeX-1;
      if (_x2 >= _sizeX)_x2 = _sizeX-1;
      if (_y1 >= _sizeY)_y1 = _sizeY-1;
      if (_y2 >= _sizeY)_y2 = _sizeY-1;

      int nCols = _y2-_y1+1;
      int nRows = _x2-_x1+1;
      int nextRow = (_sizeX-nCols)*4;
      unsigned char *startIterData = _data + (_y1*_sizeX+_x1)*4;

      for (int z=0; z<4; ++z)
      {
        _iterMedianSelect = _medianSelect;
        {
          _iterData = startIterData + z;
          for (int ySub=_y1; ySub<=_y2; ++ySub)
          {
            for (int xSub=_x1; xSub<=_x2; ++xSub)
            {
              *_iterMedianSelect++ = *_iterData;
              _iterData+=4;
            }
            _iterData += nextRow;
          }
        }
        *_outData++ = quickSelect(_medianSelect, nRows*nCols);
      }

    }

  }


}
