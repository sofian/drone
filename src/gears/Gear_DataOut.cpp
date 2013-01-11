/* Gear_DataOut.cpp
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

// Gear_DataOut.cpp: implementation of the Gear_DataOut class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_DataOut.h"
#include "Engine.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_DataOut, Gear_DataOut, "DataOut")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_DataOut::Gear_DataOut(Schema *schema, QString uniqueName) : Gear(schema, "DataOut", uniqueName)
{

  _FileOut.open("dataout.txt");
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "Input", new SignalType(0.0f)));
}

Gear_DataOut::~Gear_DataOut()
{
  _FileOut.close();
}

bool Gear_DataOut::ready()
{
  return(_AUDIO_IN->connected());
}

void Gear_DataOut::runAudio()
{
  const float *bufferin = _AUDIO_IN->type()->data();

  int signal_blocksize = Engine::signalInfo().blockSize();
  for (int i=0;i<signal_blocksize;i++)
    _FileOut << bufferin[i] << std::endl;

}
