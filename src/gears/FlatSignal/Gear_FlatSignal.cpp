/* Gear_FlatSignal.cpp
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

#include "Gear_FlatSignal.h"
#include "Math.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Engine *engine, std::string name)
{
  return new Gear_FlatSignal(engine,name);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "FlatSignal";
  gearInfo.classification = GearClassifications::signal().transform().instance();
  return gearInfo;
}
}

Gear_FlatSignal::Gear_FlatSignal(Engine *engine, std::string name)
  : GearConverter<ValueType, SignalType>(engine, "FlatSignal", name)
{
  _PLUG_IN->hintType()->setValue(0.0f);
  _PLUG_IN->hintType()->setMinValue(-1.0f);
  _PLUG_IN->hintType()->setMaxValue(1.0f);
}

void Gear_FlatSignal::convert()
{
  _PLUG_OUT->type()->fill(_PLUG_IN->type()->value());
}
