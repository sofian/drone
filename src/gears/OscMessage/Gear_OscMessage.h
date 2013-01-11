/* Gear_OscMessage.h
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

#ifndef GEAR_OSCMESSAGE_INCLUDED
#define GEAR_OSCMESSAGE_INCLUDED

#include "Gear.h"
#include "StringType.h"
#include "ValueType.h"
#include "ListType.h"
#include "OscMessageType.h"


class Gear_OscMessage : public Gear  
{
public:
  Gear_OscMessage(Schema *schema, QString uniqueName);
  virtual ~Gear_OscMessage();

  void runVideo();
	
protected:
		
  PlugIn<StringType> *_path;
  PlugIn<ListType> *_args;
	
  PlugOut<OscMessageType> *_OSC_OUT;
	
};

#endif 
