/* Gear_Tempo.cpp
 * Copyright (C) 2005 Jean-Sebastien Senecal
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

#include "Gear_Tempo.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

#include "DroneMath.h"

extern "C" {
Gear* makeGear(Schema *schema, QString uniqueName)
{
  return new Gear_Tempo(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Tempo";
  gearInfo.classification = GearClassifications::signal().generator().instance();
  return gearInfo;
}
}

Gear_Tempo::Gear_Tempo(Schema *schema, QString uniqueName) : Gear(schema, "Tempo", uniqueName)
{
  // Inputs.
  addPlug(_FREQUENCY_IN = new PlugIn<ValueType>(this, "Freq", false, new ValueType(1, 0, 1)));
  addPlug(_AMPLITUDE_IN = new PlugIn<ValueType>(this, "Amp", false, new ValueType(1, 0, 1)));
  addPlug(_RESET_PHASE_IN = new PlugIn<ValueType>(this, "Reset", false, new ValueType(0, 0, 0)));
  
  // Outputs.
  addPlug(_BEAT_OUT = new PlugOut<ValueType>(this, "Beat", true));
}

Gear_Tempo::~Gear_Tempo()
{
}

void Gear_Tempo::internalInit()
{
  _nTicks = 0;
}

#include <iostream>
void Gear_Tempo::runAudio()
{
  if (_RESET_PHASE_IN->type()->boolValue() || _nTicks <= 0)
  {
    _BEAT_OUT->type()->setValue(_AMPLITUDE_IN->type()->value());
    _nTicks = (int)(1.0f / MAX( _FREQUENCY_IN->type()->value(), FLT_MIN ));
  }
  else
    _BEAT_OUT->type()->setValue(0.0f);

  _nTicks--;
}
