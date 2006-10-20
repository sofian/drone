/* Gear_RandomValue.cpp
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


#include "Gear_RandomValue.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_RandomValue(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "RandomValue";
  gearInfo.classification = GearClassifications::unclassified().instance();
  return gearInfo;
}
}

Gear_RandomValue::Gear_RandomValue(Schema *schema, std::string uniqueName) : 
  Gear(schema, "RandomValue", uniqueName)
{
  addPlug(_NEXT = new PlugIn<ValueType>(this, "Next", false, new ValueType(1, 0, 1)));
  addPlug(_NUMBER = new PlugOut<ValueType>(this, "Rand", true));
  Random::seed();
  _NUMBER->type()->setValue(Random::uniform());
}

Gear_RandomValue::~Gear_RandomValue()
{

}

void Gear_RandomValue::runVideo()
{
  if (_NEXT->type()->boolValue())
    _NUMBER->type()->setValue(Random::uniform());
}

