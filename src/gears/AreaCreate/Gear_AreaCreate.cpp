/* Gear_PlaceArea.cpp
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

#include "Gear_PlaceArea.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_PlaceArea, Gear_PlaceArea, "PlaceArea")

Gear_PlaceArea::Gear_PlaceArea(Schema *schema, std::string uniqueName) : Gear(schema, "PlaceArea", uniqueName)
{
  addPlug(_H_POSITION_IN = new PlugIn<ValueType>(this, "X", false, new ValueType(0, 0, 352)));
  addPlug(_V_POSITION_IN = new PlugIn<ValueType>(this, "Y", false, new ValueType(0, 0, 288)));
  addPlug(_WIDTH_IN = new PlugIn<ValueType>(this, "Width", false, new ValueType(352, 1, 1024)));
  addPlug(_HEIGHT_IN = new PlugIn<ValueType>(this, "Height", false, new ValueType(288, 1, 768)));
  addPlug(_AREA_OUT = new PlugOut<AreaArrayType>(this, "Area", true));
}

Gear_PlaceArea::~Gear_PlaceArea()
{

}

void Gear_PlaceArea::internalInit()
{
  _AREA_OUT->type()->resize(1);
  _AREA_OUT->type()->operator[](0) = Area();
}

void Gear_PlaceArea::runVideo()
{
  _area = _AREA_OUT->type()->data();
  _area->x0 = MAX(_H_POSITION_IN->type()->value(), 0.0f);
  _area->y0 = MAX(_V_POSITION_IN->type()->value(), 0.0f);
  _area->x1 = MAX(_WIDTH_IN->type()->value(), 0.0f);
  _area->y1 = MAX(_HEIGHT_IN->type()->value(), 0.0f);
}


