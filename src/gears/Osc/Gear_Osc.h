/* Gear_Osc.h
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

#ifndef GEAR_OSC_INCLUDED
#define GEAR_OSC_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "ValueType.h"

class Gear_Osc : public Gear
{
public:
  Gear_Osc();
  virtual ~Gear_Osc();

  void runAudio();

protected:
  void internalInit();
  
private:
  // Inputs.
  PlugIn<ValueType> *_FREQ_IN;
  PlugIn<ValueType> *_AMP_IN;
  PlugIn<ValueType> *_PHASE_IN;
  
  // Outputs.
  PlugOut<ValueType> *_VALUE_OUT;
  PlugOut<SignalType> *_SIGNAL_OUT;

  static float _osc(float t, float amp, float freq, float phaseCorrection);

  float _oldFreq;
  float _phaseCorrection;

  // TODO: it would be nice that a Gear can send us its current time ie. Gear::currentTime().
  Time_T _currentOscTime;

};

#endif
