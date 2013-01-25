/* Gear_StringSource.cpp
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


#include "Gear_StringSource.h"
#include "Engine.h"

#include "GearMaker.h"

#include <string>

extern "C" {
Gear* makeGear()
{
  return new Gear_StringSource();
}


}

const QString Gear_StringSource::SETTING_STRING = "Source";

Gear_StringSource::Gear_StringSource() : 
  Gear("StringSource")
{
  addPlug(_STR_OUT = new PlugOut<StringType>(this, "str", false, new StringType("")));
  _settings.add(Property::FILENAME, SETTING_STRING)->valueStr("");

  _currentString = "";
}

Gear_StringSource::~Gear_StringSource()
{

}

void Gear_StringSource::onUpdateSettings()
{
  _currentString = _settings.get(SETTING_STRING)->valueStr();
}

void Gear_StringSource::runVideo()
{
  _STR_OUT->type()->setValue(_currentString);
}

