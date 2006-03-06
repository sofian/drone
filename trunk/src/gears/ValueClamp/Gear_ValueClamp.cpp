/* Gear_ValueClamp.cpp
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


#include "Gear_ValueClamp.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_ValueClamp(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "ValueClamp";
  gearInfo.classification = GearClassifications::unclassified().instance();
  return gearInfo;
}
}

Gear_ValueClamp::Gear_ValueClamp(Schema *schema, std::string uniqueName) : 
  Gear(schema, "ValueClamp", uniqueName)
{
  addPlug(_VALUE_IN = new PlugIn<ValueType>(this, "In", false, new ValueType(0.5f, 0, 1)));
  addPlug(_MIN_IN = new PlugIn<ValueType>(this, "Min", false, new ValueType(0, 0, 1)));
  addPlug(_MAX_IN = new PlugIn<ValueType>(this, "Max", false, new ValueType(1, 0, 1)));
  addPlug(_VALUE_OUT = new PlugOut<ValueType>(this, "Out", true));
}

Gear_ValueClamp::~Gear_ValueClamp()
{

}

void Gear_ValueClamp::runVideo()
{
  _VALUE_OUT->type()->setValue(CLAMP(_VALUE_IN->type()->value(), _MIN_IN->type()->value(), _MAX_IN->type()->value()));
}

