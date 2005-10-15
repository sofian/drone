/* Gear_TA_TravelAgent.h
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
#include "TA_DataType.h"
#include "VideoChannelType.h"

class Gear_TA_TravelAgent : public Gear
{
  static const std::string SETTING_FILENAME;
  
public:  
  Gear_TA_TravelAgent(Schema *schema, std::string uniqueName);
  virtual ~Gear_TA_TravelAgent();

  void runVideo();

protected:
  void onUpdateSettings();

public:
  // PlugIn<TA_DataType> *_TA_DATA_IN;
  PlugIn<VideoChannelType> *_GRID_IN;
  PlugIn<ValueType> *_ENERGY_DECAY;
  PlugIn<ValueType> *_ENERGY_CONSUMPTION;
  PlugIn<ValueType> *_ENERGY_GRID;
  PlugIn<ValueType> *_MOVE_ALLOWED;
  
  
  PlugOut<TA_DataType> *_TA_DATA_OUT;

private:
  int _currentSpot;
};

#endif
