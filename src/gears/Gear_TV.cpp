#include "Gear_TV.h"
#include "GearGui_TV.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_TV, Gear_TV, "TV")

Gear_TV::Gear_TV(Engine *engine, std::string name) : Gear(engine, "TV", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
}

Gear_TV::~Gear_TV()
{

}

bool Gear_TV::ready()
{
  return(_VIDEO_IN->connected());
}

void Gear_TV::runVideo()
{    
}

GearGui *Gear_TV::createGearGui(QCanvas *canvas)
{    
  return new GearGui_TV(this, canvas);
}

