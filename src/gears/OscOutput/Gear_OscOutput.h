/* Gear_OscOutput.h
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

#ifndef GEAR_OSCOUTPUT_INCLUDED
#define GEAR_OSCOUTPUT_INCLUDED

#include "Gear.h"
#include "StringType.h"
#include "OscMessageType.h"

#include "lo/lo.h"

class Gear_OscOutput : public Gear  
{
public:
  Gear_OscOutput();
  virtual ~Gear_OscOutput();

  void runVideo();

private:

  PlugIn<StringType> *_IP;			
  PlugIn<StringType> *_PORT;
  PlugIn<OscMessageType> *_OSC_IN;
	
};

#endif 
