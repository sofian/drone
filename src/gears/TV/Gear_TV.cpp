/* Gear_TV.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Gear_TV.h"
#include "GearGui_TV.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Engine *engine, std::string name)
{
  return new Gear_TV(engine,name);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "TV";
  gearInfo.classification = GearClassifications::control().instance();
  return gearInfo;
}
}

Gear_TV::Gear_TV(Engine *engine, std::string name) : Gear(engine, "TV", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
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

