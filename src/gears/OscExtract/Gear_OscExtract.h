/* Gear_OscExtract.h
 * Copyright (C) 2012 Jean-Sebastien Senecal
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

#ifndef GEAR_OSCEXTRACT_INCLUDED
#define GEAR_OSCEXTRACT_INCLUDED

#include "Gear.h"
#include "OscMessageType.h"

#include "lo/lo.h"

class Gear_OscExtract : public Gear
{
public:
  Gear_OscExtract(Schema *schema, std::string uniqueName);
  virtual ~Gear_OscExtract();

  void runVideo();
	
private:
			
	PlugIn<OscMessageType> *_OSC_IN;
	PlugOut<StringType> *_PATH_OUT;
  PlugOut<ListType> *_ARGS_OUT;
};

#endif 
