/* Gear_TA_DataSource.cpp
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
#include "Gear_TA_DataSource.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_DataSource(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_DataSource";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

const std::string Gear_TA_DataSource::SETTING_FILENAME = "Filename";

Gear_TA_DataSource::Gear_TA_DataSource(Schema *schema, std::string uniqueName) : 
Gear(schema, "TA_DataSource", uniqueName)
{    
  addPlug(_TA_DATA_OUT = new PlugOut<TA_DataType>(this, "DataOut", false));
  
  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    
	_TA_DATA_OUT->sleeping(true);
}

Gear_TA_DataSource::~Gear_TA_DataSource()
{
}

void Gear_TA_DataSource::onUpdateSettings()
{
  std::cout << "opening file : " << _settings.get(SETTING_FILENAME)->valueStr().c_str() << std::endl;

  _TA_DATA_OUT->type()->load(_settings.get(SETTING_FILENAME)->valueStr());
  
	_TA_DATA_OUT->sleeping(false);
}

void Gear_TA_DataSource::runVideo()
{
  // dummy
  TA_DataType *out = _TA_DATA_OUT->type();
  out->clear();
  out->insert(std::make_pair("cire", new TA_CityVertex(3,0)));
  out->insert(std::make_pair("gry", new TA_CityVertex(1,5)));
  out->addEdge("cire", "gry");
}



