/* Gear_RandomValue.h
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

#ifndef GEAR_RandomValue_INCLUDED
#define GEAR_RandomValue_INCLUDED

#include "Gear.h"
#include "ValueType.h"
#include "Random.h"

class Gear_RandomValue : public Gear  
{
public:
  Gear_RandomValue(Schema *schema, QString uniqueName);
  virtual ~Gear_RandomValue();

  void runVideo();

protected:
  PlugIn<ValueType> *_NEXT;

  PlugOut<ValueType> *_NUMBER;
};

#endif 
