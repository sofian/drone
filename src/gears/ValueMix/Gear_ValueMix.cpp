/* Gear_ValueMix.cpp
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


#include "Gear_ValueMix.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_ValueMix(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "ValueMix";
  gearInfo.classification = GearClassifications::signal().transform().instance();
  return gearInfo;
}
}

Gear_ValueMix::Gear_ValueMix(Schema *schema, std::string uniqueName) : Gear(schema, "ValueMix", uniqueName)
{

  addPlug(_VALUE_IN1 = new PlugIn<ValueType>(this, "In1", true, new ValueType(0.0f)));
  addPlug(_VALUE_IN2 = new PlugIn<ValueType>(this, "In2", true, new ValueType(0.0f)));
  addPlug(_BLEND= new PlugIn<ValueType>(this, "Blend", false, new ValueType(0.0f,0.0f,1.0f)));

  addPlug(_VALUE_OUT = new PlugOut<ValueType>(this, "Out", true));
}

Gear_ValueMix::~Gear_ValueMix()
{

}

void Gear_ValueMix::runVideo()
{
  _VALUE_OUT->type()->setValue(_VALUE_IN1->type()->value() * _BLEND->type()->value() + _VALUE_IN2->type()->value() * (1 - _BLEND->type()->value()));

}
