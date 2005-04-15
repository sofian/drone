/* Gear_TapTempo.h
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

#ifndef GEAR_TapTempo_INCLUDED
#define GEAR_TapTempo_INCLUDED

#include <deque>
#include "Gear.h"
#include "SignalType.h"
#include "ValueType.h"

class Gear_TapTempo : public Gear  
{
public:
  Gear_TapTempo(Schema *schema, std::string uniqueName);
  virtual ~Gear_TapTempo();

  void runAudio();

  bool ready();

protected:
  void internalInit();
  
private:
  PlugIn<ValueType> *_TAP_IN;
  PlugIn<ValueType> *_DECAY_IN;
  
  PlugOut<ValueType> *_VALUE_OUT;

  float _lastTapTime;
  float _estimatedTempo;
  float _scheduledBeatTimeStamp;

};

#endif 
