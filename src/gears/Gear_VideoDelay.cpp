#include "Gear_VideoDelay.h"
#include "Engine.h"
#include "CircularBuffer.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_VideoDelay, Gear_VideoDelay, "VideoDelay")

Gear_VideoDelay::Gear_VideoDelay(Engine *engine, std::string name) : Gear(engine, "VideoDelay", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_MEMORY = new PlugIn<ValueType>(this, "Memory", new ValueType(125, 0, 125)));
  addPlug(_DELAY = new PlugIn<ValueType>(this, "Delay", new ValueType(0,-124,0)));
  addPlug(_RECORD = new PlugIn<ValueType>(this, "Rec", new ValueType(1,0,1)));

  circbuf = new CircularBuffer<RGBA>(BLACK_RGBA);

}

Gear_VideoDelay::~Gear_VideoDelay()
{
}

void Gear_VideoDelay::init()
{
}

bool Gear_VideoDelay::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_VideoDelay::runVideo()
{
  _image = _VIDEO_IN->type();

  _sizeY = _image->height();
  _sizeX = _image->width();

  _delay = CLAMP(_DELAY->type()->intValue(),-_memory+1,0);
  _memory=_MEMORY->type()->intValue();

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_sizeX, _sizeY);
 
  circbuf->resize(_sizeY*_sizeX, _memory);
  if(_RECORD->type()->intValue())
    circbuf->append(_image->data());

  circbuf->fillVectorFromBlock(_outImage, _delay);
  
}
