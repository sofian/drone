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
#include <iostream>
#include <math.h>

extern "C" {
Gear* makeGear(Engine *engine, std::string name)
{
  return new Gear_Osc(engine,name);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Osc";
  gearInfo.classification = GearClassifications::signal().generator().instance();
  return gearInfo;
}
}

Gear_Osc::Gear_Osc(Engine *engine, std::string name) : 
Gear(engine, "Oscillator", name),
_phaseCorrection(0.0f)
{
  addPlug(_PARAM_FREQ = new PlugIn<ValueType>(this, "Freq", new ValueType(440.0f,0.0f,1000.0f)));
  addPlug(_PARAM_AMP = new PlugIn<ValueType>(this, "Amp", new ValueType(1.0f,0.0f,2.0f)));
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
  float freq  = _PARAM_FREQ->type()->value();
  float amp  = _PARAM_AMP->type()->value();
  
   _currentTime = _engine->currentTime();
  
  if (_oldFreq!=freq)
  {             
    _phaseCorrection = (_oldFreq*_currentTime* 6.28318f) - (freq*_currentTime* 6.28318f) + _phaseCorrection;
    _oldFreq=freq;
  }

  _VALUE_OUT->type()->setValue(amp * cos(freq * _currentTime * 6.28318f + _phaseCorrection));
}
