/* Gear_PlaceArea.h
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#ifndef GEAR_PLACEAREA_INCLUDED
#define GEAR_PLACEAREA_INCLUDED


#include "Gear.h"
#include "ValueType.h"
#include "AreaArrayType.h"


class Gear_PlaceArea : public Gear
{
public:

  Gear_PlaceArea(Schema *schema, std::string uniqueName);
  virtual ~Gear_PlaceArea();

  void init();
  void runVideo();
  bool ready();

private:
  PlugOut<AreaArrayType> *_AREA_OUT;
  PlugIn<ValueType> *_H_POSITION_IN;
  PlugIn<ValueType> *_V_POSITION_IN;
  PlugIn<ValueType> *_WIDTH_IN;
  PlugIn<ValueType> *_HEIGHT_IN;

  Area *_area;
};

#endif
