/* Gear_AreaScale.cpp
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

#include "Gear_AreaScale.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_AreaScale(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AreaScale";
  gearInfo.classification = GearClassifications::video().mask().instance();
  return gearInfo;
}
}

Gear_AreaScale::Gear_AreaScale(Schema *schema, std::string uniqueName) : Gear(schema, "AreaScale", uniqueName)
{
  addPlug(_AREA_OUT = new PlugOut<AreaArrayType>(this, "AreaOut", true));

  addPlug(_AREA_IN = new PlugIn<AreaArrayType>(this, "AreaIn", true));
  addPlug(_X_SCALE_IN = new PlugIn<ValueType>(this, "XScale", false, new ValueType(1, 0, 2)));
  addPlug(_Y_SCALE_IN = new PlugIn<ValueType>(this, "YScale", false, new ValueType(1, 0, 2)));
}

Gear_AreaScale::~Gear_AreaScale()
{

}

void Gear_AreaScale::internalInit()
{
}

void Gear_AreaScale::runVideo()
{
  _areaArrayIn = _AREA_IN->type();
  _areaArrayOut = _AREA_OUT->type();
  _AREA_OUT->type()->resize(_AREA_IN->type()->size());

  AreaArrayType::const_iterator in = _areaArrayIn->begin();
  AreaArrayType::iterator out = _areaArrayOut->begin();
  
  for ( ; in != _areaArrayIn->end(); ++in, ++out)
  {
    out->width  = ((float)in->width) * _X_SCALE_IN->type()->intValue();
    out->height = ((float)in->height) * _Y_SCALE_IN->type()->intValue();
  }
}


