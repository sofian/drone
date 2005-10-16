/* Gear_StringGen.cpp
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


#include "Gear_StringGen.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_StringGen(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "StringGen";
  gearInfo.classification = GearClassifications::protocol().osc().instance();
  return gearInfo;
}
}

Gear_StringGen::Gear_StringGen(Schema *schema, std::string uniqueName) : 
  Gear(schema, "StringGen", uniqueName)
{

  addPlug(_GO = new PlugIn<ValueType>(this, "go", false, new ValueType(0)));

  addPlug(_STR_OUT = new PlugOut<StringType>(this, "str", false, new StringType("aaa")));
	_STR_OUT->sleeping(true);
}

Gear_StringGen::~Gear_StringGen()
{

}

void Gear_StringGen::runVideo()
{
	if (_GO->type()->value()>0.0f)
		_STR_OUT->sleeping(false);		
	else
		_STR_OUT->sleeping(true);		
		
}

