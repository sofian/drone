/* Gear_StringFormat.h
 * Copyright (C) 2005 Jean-Sebastien Senecal
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

#ifndef GEAR_STRINGFORMAT_INCLUDED
#define GEAR_STRINGFORMAT_INCLUDED

#include "Gear.h"
#include "StringType.h"
#include "ValueType.h"
#include "ListType.h"


class Gear_StringFormat : public Gear  
{
public:
  Gear_StringFormat();
  virtual ~Gear_StringFormat();

  void runVideo();

protected:

  PlugIn<StringType> *_FORMAT;
  PlugIn<ListType> *_PARAMS;
	
  PlugOut<StringType> *_STRING;
  
  char _buffer[1024];
};

#endif 
