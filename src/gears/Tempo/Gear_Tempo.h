/* Gear_Tempo.h
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

#ifndef GEAR_TEMPO_INCLUDED
#define GEAR_TEMPO_INCLUDED

#include "Gear.h"
#include "ValueType.h"

class Gear_Tempo : public Gear
{
public:
  Gear_Tempo();
  virtual ~Gear_Tempo();

  void runAudio();

protected:
  void internalInit();
  
private:
  // Inputs.
  PlugIn<ValueType> *_FREQUENCY_IN;
  PlugIn<ValueType> *_AMPLITUDE_IN;
  PlugIn<ValueType> *_RESET_PHASE_IN;

  // Outputs.
  PlugOut<ValueType> *_BEAT_OUT;

  // Local vars.
  int _nTicks; // number of ticks before next beat
    
};

#endif
