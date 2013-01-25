/* Gear_Enveloppe.h
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

#ifndef GEAR_ENVELOPPE_INCLUDED
#define GEAR_ENVELOPPE_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "ValueType.h"

class PlugSignal;

class Gear_Enveloppe : public Gear  
{
public:
	Gear_Enveloppe();
	virtual ~Gear_Enveloppe();

  void runAudio();
  
  virtual void prePlay();

private:
  PlugIn<SignalType> *_AUDIO_IN;
  PlugIn<ValueType> *_PARAM_RELEASE, *_PARAM_ATTACK;
  PlugOut<SignalType> *_AUDIO_OUT; 
  
  Signal_T _lastenvel,_envel;
  Signal_T _envin;

};

#endif 
