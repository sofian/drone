/* Gear_FilterSignal.h
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

#ifndef GEAR_FILTERSIGNAL_INCLUDED
#define GEAR_FILTERSIGNAL_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "FrequencyAnalyzer.h"

class PlugSignal;

class Gear_FilterSignal : public Gear  
{
public:
  Gear_FilterSignal(Engine *engine, std::string name);
  virtual ~Gear_FilterSignal();

  void runAudio();

  PlugIn<SignalType>* AUDIO_IN(){return _AUDIO_IN;};    
  PlugOut<SignalType>* AUDIO_OUT(){return _AUDIO_OUT;};

  bool ready();
  void init();

private:
  PlugIn<SignalType> *_AUDIO_IN;
  PlugOut<SignalType> *_AUDIO_OUT;

  SignalType _convolutionShape;
  FrequencyAnalyser *_freqAnalyzer;

};

#endif 
