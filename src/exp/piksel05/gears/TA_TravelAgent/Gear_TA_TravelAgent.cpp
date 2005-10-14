/* Gear_TA_TravelAgent.cpp
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
//inspired from Using libavformat and libavcodec by Martin Böhme (boehme@inb.uni-luebeckREMOVETHIS.de) 

#include <iostream>
#include "Gear_TA_TravelAgent.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_TravelAgent(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_TravelAgent";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

const std::string Gear_TA_TravelAgent::SETTING_FILENAME = "Filename";

Gear_TA_TravelAgent::Gear_TA_TravelAgent(Schema *schema, std::string uniqueName) : 
Gear(schema, "TA_TravelAgent", uniqueName)
{
  addPlug(_TA_DATA_IN = new PlugIn<TA_DataType>(this, "DataIn", false));
  addPlug(_GRID_IN = new PlugIn<VideoChannelType>(this, "GridIn", false));
  
  addPlug(_ENERGY_DECAY = new PlugIn<ValueType>(this, "E-Decay", false, new ValueType(0.001, 0, 1)));
  addPlug(_ENERGY_CONSUMPTION = new PlugIn<ValueType>(this, "E-Consum", false, new ValueType(0.001, 0, 1)));
  addPlug(_ENERGY_GRID = new PlugIn<ValueType>(this, "E-Grid", false, new ValueType(0.001, 0, 1)));
  addPlug(_MOVE_ALLOWED = new PlugIn<ValueType>(this, "MoveOK", false, new ValueType(1, 0, 1)));

  addPlug(_TA_DATA_OUT = new PlugOut<TA_DataType>(this, "DataOut", false));
  
  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    
	_TA_DATA_OUT->sleeping(true);
}

Gear_TA_TravelAgent::~Gear_TA_TravelAgent()
{
}

void Gear_TA_TravelAgent::onUpdateSettings()
{
  std::cout << "opening file : " << _settings.get(SETTING_FILENAME)->valueStr().c_str() << std::endl;

  _TA_DATA_OUT->type()->load(_settings.get(SETTING_FILENAME)->valueStr());
  
	_TA_DATA_OUT->sleeping(false);
}

void Gear_TA_TravelAgent::runVideo()
{
}



