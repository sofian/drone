/* Gear_AffineTransform.cpp
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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


#include "Gear_AffineTransform.h"
#include "Engine.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_AffineTransform, Gear_AffineTransform, "AffineTransform")

Gear_AffineTransform::Gear_AffineTransform(Engine *engine, std::string name) : Gear(engine, "AffineTransform", name)
{

  addPlug(_VALUE_IN = new PlugIn<ValueType>(this, "In", new ValueType(0.0f)));
  addPlug(_SCALE= new PlugIn<ValueType>(this, "Scale", new ValueType(1.0f,0.0f,10.0f)));
  addPlug(_OFFSET= new PlugIn<ValueType>(this, "Offset", new ValueType(0.0f,-10.0f,10.0f)));
  
  addPlug(_VALUE_OUT = new PlugOut<ValueType>(this, "Out"));
}

Gear_AffineTransform::~Gear_AffineTransform()
{

}

bool Gear_AffineTransform::ready()
{
  return(_VALUE_IN->connected() && _VALUE_OUT->connected());
}

void Gear_AffineTransform::runVideo()
{
  _VALUE_OUT->type()->setValue(_VALUE_IN->type()->value() * _SCALE->type()->value() + _OFFSET->type()->value());

}
