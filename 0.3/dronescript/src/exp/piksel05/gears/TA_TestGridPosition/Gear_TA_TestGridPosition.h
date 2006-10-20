/* Gear_TA_TestGridPosition.h
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

#ifndef GEAR_TA_DATASOURCE_INCLUDED
#define GEAR_TA_DATASOURCE_INCLUDED

#include "Gear.h"
#include "ValueType.h"
#include "VideoChannelType.h"
#include "VideoRGBAType.h"

#define TA_GRID_WIDTH 8
#define TA_GRID_HEIGHT 8

class Gear_TA_TestGridPosition : public Gear
{
public:  
  Gear_TA_TestGridPosition(Schema *schema, std::string uniqueName);
  virtual ~Gear_TA_TestGridPosition();

  void runVideo();
  
public:
  PlugIn<ValueType> *_X0_IN;
  PlugIn<ValueType> *_Y0_IN;
  PlugIn<ValueType> *_X1_IN;
  PlugIn<ValueType> *_Y1_IN;
  
  PlugOut<VideoChannelType> *_GRID_OUT;
  PlugOut<VideoRGBAType> *_RGBA_GRID_OUT;
};

#endif
