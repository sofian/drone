/* Gear_AreaCreate.cpp
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

#include "Gear_AreaCreate.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, QString uniqueName)
{
  return new Gear_AreaCreate(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AreaCreate";
  gearInfo.classification = GearClassifications::video().mask().instance();
  return gearInfo;
}
}


Gear_AreaCreate::Gear_AreaCreate(Schema *schema, QString uniqueName) : Gear(schema, "AreaCreate", uniqueName)
{
  addPlug(_H_POSITION_IN = new PlugIn<ValueType>(this, "X", false, new ValueType(0, 0, 352)));
  addPlug(_V_POSITION_IN = new PlugIn<ValueType>(this, "Y", false, new ValueType(0, 0, 288)));
  addPlug(_WIDTH_IN = new PlugIn<ValueType>(this, "Width", false, new ValueType(352, 1, 1024)));
  addPlug(_HEIGHT_IN = new PlugIn<ValueType>(this, "Height", false, new ValueType(288, 1, 768)));
  addPlug(_AREA_OUT = new PlugOut<AreaType>(this, "Area", true));
}

Gear_AreaCreate::~Gear_AreaCreate()
{

}

void Gear_AreaCreate::runVideo()
{
  _areaOut = _AREA_OUT->type();
  _areaOut->setOrigin(_H_POSITION_IN->type()->intValue(), _V_POSITION_IN->type()->intValue());
  _areaOut->resize(MAX(_WIDTH_IN->type()->intValue(), 0), MAX(_HEIGHT_IN->type()->intValue(), 0));
}


