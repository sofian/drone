/* Gear_OscFilter.cpp
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


#include "Gear_OscFilter.h"
#include "Engine.h"



extern "C" {
Gear* makeGear()
{
  return new Gear_OscFilter();
}


}

Gear_OscFilter::Gear_OscFilter() : 
	Gear("OscFilter")
{
	addPlug(_OSC_IN = new PlugIn<OscMessageType>(this, "Osc In", true));
	addPlug(_PATH_IN = new PlugIn<StringType>(this, "path", false, new StringType("/")));

	addPlug(_OSC_OUT = new PlugOut<OscMessageType>(this, "Osc Out", true));
}

Gear_OscFilter::~Gear_OscFilter()
{

}

void Gear_OscFilter::runVideo()
{
	if (_PATH_IN->type()->value() == _OSC_IN->type()->path().value())
	{
		_OSC_OUT->type()->setPath(_OSC_IN->type()->path());
		_OSC_OUT->type()->setArgs(_OSC_IN->type()->args());
		_OSC_OUT->sleeping(false);
	}
	else
		_OSC_OUT->sleeping(true);
}
