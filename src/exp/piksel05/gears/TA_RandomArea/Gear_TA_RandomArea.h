/* Gear_TA_RandomArea.h
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

#ifndef GEAR_TA_RandomArea_INCLUDED
#define GEAR_TA_RandomArea_INCLUDED

#include "Gear.h"
#include "AreaType.h"
#include "ValueType.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

class Gear_TA_RandomArea : public Gear  
{
public:
  Gear_TA_RandomArea(Schema *schema, std::string uniqueName);
  virtual ~Gear_TA_RandomArea();

  void runVideo();

protected:
  PlugIn<ValueType> *_NEXT;

  PlugOut<AreaType> *_AREA;
};

#endif 
