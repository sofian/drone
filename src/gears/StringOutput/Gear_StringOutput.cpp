/* Gear_StringOutput.cpp
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

#include "Gear_StringOutput.h"
#include "GearGui_StringOutput.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_StringOutput(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "StringOutput";
  gearInfo.classification = GearClassifications::control().instance();
  return gearInfo;
}
}

Gear_StringOutput::Gear_StringOutput(Schema *schema, std::string uniqueName) : 
	Gear(schema, "StringOutput", uniqueName)
{
  addPlug(_STRING_IN = new PlugIn<StringType>(this, "string", true));
}

Gear_StringOutput::~Gear_StringOutput()
{

}

void Gear_StringOutput::runVideo()
{    
}

std::string Gear_StringOutput::getString()
{
	return _STRING_IN->type()->value();
}


GearGui *Gear_StringOutput::createGearGui(QCanvas *canvas)
{    
  return new GearGui_StringOutput(this, canvas);
}

