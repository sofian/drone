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
#include "Gear_TA_MapNavigator.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_MapNavigator(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_MapNavigator";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

Gear_TA_MapNavigator::Gear_TA_MapNavigator(Schema *schema, std::string uniqueName) : 
Gear(schema, "TA_MapNavigator", uniqueName)
{

  addPlug(_DATA_IN = new   PlugIn<TA_DataType>(this,"DATA",true)); 
  addPlug(_HOTSPOT = new   PlugIn<ValueType>(this,"HOTSPOT",true));
  
  addPlug(_SCALE  = new   PlugOut<ValueType>(this,"SCALE",true));
  addPlug(_XOFF  = new   PlugOut<ValueType>(this,"XOFF",true));
  addPlug(_YOFF  = new   PlugOut<ValueType>(this,"YOFF",true));
  addPlug(_ROTATE  = new   PlugOut<ValueType>(this,"ROTATE",true));
  addPlug(_RAX  = new   PlugOut<ValueType>(this,"RAX",true));
  addPlug(_RAY  = new   PlugOut<ValueType>(this,"RAY",true));
  addPlug(_REACHED  = new   PlugOut<ValueType>(this,"REACHED",true));

}

Gear_TA_MapNavigator::~Gear_TA_MapNavigator()
{
}

void Gear_TA_MapNavigator::runVideo()
{
}



