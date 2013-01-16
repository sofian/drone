/* Gear_AreaTranslate.cpp
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

#include "Gear_AreaTranslate.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

extern "C" {
Gear* makeGear()
{
  return new Gear_AreaTranslate();
}


}

Gear_AreaTranslate::Gear_AreaTranslate() : Gear("AreaTranslate")
{
  addPlug(_AREA_OUT = new PlugOut<AreaType>(this, "AreaOut", true));

  addPlug(_AREA_IN = new PlugIn<AreaType>(this, "AreaIn", true));
  addPlug(_X_IN = new PlugIn<ValueType>(this, "X", false, new ValueType(0, -10, 10)));
  addPlug(_Y_IN = new PlugIn<ValueType>(this, "Y", false, new ValueType(0, -10, 10)));
}

Gear_AreaTranslate::~Gear_AreaTranslate()
{

}

void Gear_AreaTranslate::internalInit()
{
}

void Gear_AreaTranslate::runVideo()
{
  _areaIn = _AREA_IN->type();
  _areaOut = _AREA_OUT->type();
  
  _areaOut->setOrigin(_areaIn->x0() + _X_IN->type()->intValue(),
                      _areaIn->y0() + _Y_IN->type()->intValue());
  _areaOut->resize(_areaIn->width(), _areaIn->height());
}


