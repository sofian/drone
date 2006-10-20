/* Gear_Gain.cpp
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

// Gear_Gain.cpp: implementation of the Gear_Gain class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_Gain.h"
#include "Engine.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_Gain, Gear_Gain, "Gain")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_Gain::Gear_Gain(Schema *schema, std::string uniqueName) : Gear(schema, "Gain", uniqueName)
{

  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "In", true, new SignalType(0.0f)));
  addPlug(_PARAM_GAIN = new PlugIn<SignalType>(this, "Gain", false, new SignalType(1.0f)));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "Out", true));
}

Gear_Gain::~Gear_Gain()
{

}

void Gear_Gain::runAudio()
{
  const float *bufferin = _AUDIO_IN->type()->data();
  float *bufferout = _AUDIO_OUT->type()->data();
  const float *buffergain  = _PARAM_GAIN->type()->data();

  int signal_blocksize = Engine::signalInfo().blockSize();
  for (int i=0;i<signal_blocksize;i++)
  {
    //cout << bufferin[i] << endl;
    bufferout[i] = bufferin[i] * buffergain[i];                
  }

}
