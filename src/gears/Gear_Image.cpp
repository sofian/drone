#include "Gear_Image.h"
#include "Engine.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_Image, Gear_Image, "Image")

const std::string Gear_Image::SETTING_FILENAME = "Filename";

Gear_Image::Gear_Image(Engine *engine, std::string name) : Gear(engine, "Image", name)
{
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "Out"));

  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    
}

Gear_Image::~Gear_Image()
{

}

bool Gear_Image::ready()
{
  return(_VIDEO_OUT->connected());
}


void Gear_Image::onUpdateSettings()
{
  //load filename into output
}

void Gear_Image::runVideo()
{
}

