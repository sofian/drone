/* Gear_TA_MapNavigator.h
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

#ifndef GEAR_TA_MAPNAVIGATOR_INCLUDED
#define GEAR_TA_MAPNAVIGATOR_INCLUDED


#include "Gear.h"
#include "ValueType.h"
#include "TA_DataType.h"

class Gear_TA_MapNavigator : public Gear
{
public:  

  Gear_TA_MapNavigator(Schema *schema, std::string uniqueName);
  virtual ~Gear_TA_MapNavigator();

  void runVideo();

public:
  PlugIn<TA_DataType> *_DATA_IN;
  PlugIn<ValueType> *_HOTSPOT;
  

  PlugOut<ValueType> *_SCALE;
  PlugOut<ValueType> *_XOFF;
  PlugOut<ValueType> *_YOFF;
  PlugOut<ValueType> *_ROTATE;
  PlugOut<ValueType> *_RAX;  
  PlugOut<ValueType> *_RAY;
  PlugOut<ValueType> *_REACHED;    
};

#endif
