/* Gear_Enveloppe.cpp
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

#include "Gear_Enveloppe.h"
#include "Engine.h"
#include <cmath>
#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, QString uniqueName)
{
  return new Gear_Enveloppe(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Enveloppe";
  gearInfo.classification = GearClassifications::signal().enveloppe().instance();
  return gearInfo;
}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_Enveloppe::Gear_Enveloppe(Schema *schema, QString uniqueName) : Gear(schema, "Enveloppe", uniqueName)
{    
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "In", true));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "Out", true));

  addPlug(_PARAM_RELEASE = new PlugIn<ValueType>(this, "Rel", false, new ValueType(.07f,0,1)));
  addPlug(_PARAM_ATTACK = new PlugIn<ValueType>(this, "Att", false, new ValueType(.07f,0,1)));
}

Gear_Enveloppe::~Gear_Enveloppe()
{

}

void Gear_Enveloppe::prePlay()
{
  _lastenvel=0;

}    

void Gear_Enveloppe::runAudio()
{
  const float *bufferin = _AUDIO_IN->type()->data();
  float *bufferout = _AUDIO_OUT->type()->data();
  float release = _PARAM_RELEASE->type()->value();
  float attack = _PARAM_ATTACK->type()->value();

  int samplerate = Engine::signalInfo().sampleRate();
  int signal_blocksize = Engine::signalInfo().blockSize();

  float ga = (float) exp(-1/(samplerate* attack));
  float gr = (float) exp(-1/(samplerate* release));

  for (int i=0;i<signal_blocksize;i++)
  {
    _envin = fabs(*bufferin++);

    if (_envel < _envin)
    {
      _envel *= ga;
      _envel += (1-ga)*_envin;
    } else
    {
      _envel *= gr;
      _envel += (1-gr)*_envin;
    }

    _lastenvel = bufferout[i] = _envel;
  }      
}
