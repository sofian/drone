/* Gear_AudioSource.cpp
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

#include <iostream>
#include "Gear_AudioSource.h"
#include "Engine.h"

#include "GearMaker.h"


Register_Gear(MAKERGear_AudioSource, Gear_AudioSource, "AudioSource");

const std::string Gear_AudioSource::SETTING_FILENAME = "Filename";

Gear_AudioSource::Gear_AudioSource(Schema *schema, std::string uniqueName) : Gear(schema, "AudioSource", uniqueName),
_File(NULL)
{        
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "Out"));       

  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    
}

Gear_AudioSource::~Gear_AudioSource()
{
  if (_File!=NULL)
    sf_close(_File);

}

bool Gear_AudioSource::ready()
{
  return(_File!=NULL && _AUDIO_OUT->connected());
}

void Gear_AudioSource::onUpdateSettings()
{

  if (_File!=NULL)
    sf_close(_File);

  _File = sf_open(_settings.get(SETTING_FILENAME)->valueStr().c_str(), SFM_READ, &_SFInfo);    

  if (_File==NULL)
  {
    std::cout << "error opening soundfile!" << std::endl;
    return;
  }
}


void Gear_AudioSource::runAudio()
{
  if ( sf_readf_float(_File, _AUDIO_OUT->type()->data(), Engine::signalInfo().blockSize()) == 0)
  {
    //go back to the beginning, loop
    sf_seek(_File, 0, SEEK_SET);
  }
}


