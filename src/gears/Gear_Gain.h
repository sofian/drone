/* Gear_Gain.h
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#ifndef GEAR_GAIN_INCLUDED
#define GEAR_GAIN_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "ValueType.h"

class PlugSignal;

class Gear_Gain : public Gear  
{
public:
  Gear_Gain(Engine *engine, std::string name);
  virtual ~Gear_Gain();

  void runAudio();

  bool ready();

private:
  PlugIn<SignalType> *_AUDIO_IN;
  PlugIn<SignalType> *_PARAM_GAIN;
  PlugOut<SignalType> *_AUDIO_OUT;

};

#endif 
