#include "Gear_Blur.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

#include "Math.h"

Register_Gear(MAKERGear_Blur, Gear_Blur, "Blur")

Gear_Blur::Gear_Blur(Engine *engine, std::string name) : Gear(engine, "Blur", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Amount", new ValueType(3, 0, 50)));
  _table = new SummedAreaTable<>();
}

Gear_Blur::~Gear_Blur()
{
  delete _table;
}

void Gear_Blur::init()
{
  //  _table->setImage(_VIDEO_IN->type());
}

bool Gear_Blur::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Blur::runVideo()
{
  NOTICE("In runVideo");
  _image = _VIDEO_IN->type();
  ASSERT_ERROR(_image);
    
  _outImage = _VIDEO_OUT->type();
  ASSERT_ERROR(_outImage);
      
  _outImage->resize(_image->width(), _image->height());
  
  _sizeY = _image->height();
  _sizeX = _image->width();
  
  ////////////////////////////
  _blurSize=(int) MAX(_AMOUNT_IN->type()->value(), 0.0f);

  if (_blurSize == 0)
  {
    // No blur, just copy image.
    memcpy(_outImage->data(), _image->data(), _image->size()*sizeof(RGBA));
  }
  else
  {
    ASSERT_ERROR(_table);
    ASSERT_ERROR(_sizeX >= 0);
    ASSERT_ERROR(_sizeY >= 0);
    ASSERT_ERROR(_image->data());

    // Compute the summed area table.
    _table->setTable((unsigned char*)_image->data(), _sizeX, _sizeY);
    _table->buildTable();
    
    _outData = (unsigned char*)_outImage->data();

    // Loop through the image's pixels.
    for (int y=0;y<_sizeY;y++)
    {
      for (int x=0;x<_sizeX;x++)
      {
        // The kernel's coordinates.
        _x1 = x - _blurSize - 1;
        _x2 = x + _blurSize;
        _y1 = y - _blurSize - 1;
        _y2 = y + _blurSize;

        // Get the sum in the current kernel.
        _table->getSum(_sum, _area, _x1, _y1, _x2, _y2);
        
        ASSERT_ERROR(_area >= 0);

        // Take the mean and copy it to output.
        divideVecVal(_outData, _sum, _area, SIZE_RGBA);

        // Increment iterator.
        _outData += SIZE_RGBA;
      }
    }
  }

}
