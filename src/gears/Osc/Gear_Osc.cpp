/* Gear_Osc.cpp
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

#include "Gear_Osc.h"
#include "Engine.h"
#include "GearMaker.h"
#include "Math.h"
#include "Engine.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_Osc(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Osc";
  gearInfo.classification = GearClassifications::signal().generator().instance();
  return gearInfo;
}
}

Gear_Osc::Gear_Osc(Schema *schema, std::string uniqueName) : 
  Gear(schema, "Osc", uniqueName),
  _phaseCorrection(0.0f)
{
  // Inputs.
  addPlug(_FREQ_IN = new PlugIn<ValueType>(this, "Freq", new ValueType(440.0f,0.0f,1000.0f)));
  addPlug(_AMP_IN = new PlugIn<ValueType>(this, "Amp", new ValueType(1.0f,0.0f,2.0f)));

  // Outputs.
  addPlug(_VALUE_OUT = new PlugOut<ValueType>(this, "Out"));
}

Gear_Osc::~Gear_Osc()
{

}

bool Gear_Osc::ready()
{
  return _VALUE_OUT->connected();
}

void Gear_Osc::runAudio()
{          
  float freq  = _FREQ_IN->type()->value();
  float amp   = _AMP_IN->type()->value();

  _currentTimeTimesTwicePi = Engine::currentTime()*(float)(TWICE_PI);
  
  if (_oldFreq!=freq)
  {
    _phaseCorrection += (_oldFreq-freq)*_currentTimeTimesTwicePi;
    _oldFreq=freq;
  }

  _VALUE_OUT->type()->setValue(amp * fastcos(freq * _currentTimeTimesTwicePi + _phaseCorrection));
}
