#include "Gear_Rescale.h"
#include "Engine.h"

#include "MatrixType.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_Rescale, Gear_Rescale, "Rescale")

Gear_Rescale::Gear_Rescale(Engine *engine, std::string name) : Gear(engine, "Rescale", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_WIDTH_IN = new PlugIn<ValueType>(this, "Width", new ValueType(320, 0, 640)));
  addPlug(_HEIGHT_IN = new PlugIn<ValueType>(this, "Height", new ValueType(280, 0, 560)));
}

Gear_Rescale::~Gear_Rescale()
{

}

bool Gear_Rescale::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Rescale::runVideo()
{
  _image = _VIDEO_IN->type();
  _outImage = _VIDEO_OUT->type();
  _newWidth = (int) MAX((int)_WIDTH_IN->type()->value(), 0);
  _newHeight = (int) MAX((int)_HEIGHT_IN->type()->value(), 0);
  _outImage->resize(_newWidth, _newHeight);

  rescale_image(_outImage->data(), _image->data(), _newWidth, _newHeight, _image->width(), _image->height());
}
