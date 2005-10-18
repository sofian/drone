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
#include "StringUtils.h"
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
Gear(schema, "TA_MapNavigator", uniqueName),initialized(false)
{

  addPlug(_DATA_IN = new   PlugIn<TA_DataType>(this,"DATA",true)); 
  addPlug(_HOTSPOT = new   PlugIn<ValueType>(this,"HOTSPOT",true));
  //addPlug(_HOTSPOT = new   PlugIn<ValueType>(this,"HOTSPOT",true));
  
  addPlug(_SCALE  = new   PlugOut<ValueType>(this,"SCALE",false));
  addPlug(_XOFF  = new   PlugOut<ValueType>(this,"XOFF",false));
  addPlug(_YOFF  = new   PlugOut<ValueType>(this,"YOFF",false));
  addPlug(_ROTATE  = new   PlugOut<ValueType>(this,"ROTATE",false));
  addPlug(_RAX  = new   PlugOut<ValueType>(this,"RAX",false));
  addPlug(_RAY  = new   PlugOut<ValueType>(this,"RAY",false));
  addPlug(_REACHED  = new   PlugOut<ValueType>(this,"REACHED",false));

}

Gear_TA_MapNavigator::~Gear_TA_MapNavigator()
{
}

void Gear_TA_MapNavigator::runVideo()
{
	bool moving;
	int hs_num = (int)_HOTSPOT->type()->value();	
	TA_DataType* data = _DATA_IN->type();
	TA_CityVertex ver = (*data)[hs_num];
	//data->printDebug();
	float goalx(ver.x),goaly(ver.y);
	if(!initialized)
	{
		initialized=true;	
		rax = goalx;
		ray = goaly;
	}		
	
	moving = !(fabs(rax-goalx)<1 && fabs(ray-goaly)<1);
	_REACHED->type()->setValue(moving?0.0f:1.0f);
	rax += (goalx-rax)/10;
	ray += (goaly-ray)/10;

	std::cerr<<"hs:"<<hs_num<<" g:"<<goalx<<","<<goaly<<" cur:"<<rax<<","<<ray<<" moving:"<<(moving?1:0)<<"pok:"<<fabs(rax-goalx)<<std::endl;

	_RAX->type()->setValue(rax);
	_RAY->type()->setValue(ray);
	_XOFF->type()->setValue(rax);
	_YOFF->type()->setValue(ray);
	_SCALE->type()->setValue(moving?1.0f:4.0f);
	
}



