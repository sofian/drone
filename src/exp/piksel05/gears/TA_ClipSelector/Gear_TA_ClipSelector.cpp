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
#include "Gear_TA_ClipSelector.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_ClipSelector(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_ClipSelector";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

Gear_TA_ClipSelector::Gear_TA_ClipSelector(Schema *schema, std::string uniqueName) : 
  Gear(schema, "TA_ClipSelector", uniqueName), _currentSpot(-1)
{
  addPlug(_DATA_IN = new   PlugIn<TA_DataType>(this,"DATA",true)); 
  addPlug(_HOTSPOT = new   PlugIn<ValueType>(this,"HOTSPOT",true));

  addPlug(_CURRENT_CLIP  = new   PlugOut<StringType>(this,"CURR",false));
  NOTICE("****CURRENT CLIP DOIT ETRE OBLIGATOIRE***");
  addPlug(_PREVIOUS_CLIP  = new   PlugOut<StringType>(this,"PREV",false));
  addPlug(_CLIP_CHANGED  = new   PlugOut<ValueType>(this,"CHANGED",false));
}

Gear_TA_ClipSelector::~Gear_TA_ClipSelector()
{
}

void Gear_TA_ClipSelector::runVideo()
{
  TA_DataType *data = _DATA_IN->type();
  int spot = _HOTSPOT->type()->intValue();

  if (_currentSpot == -1)
    _previousSpot = _currentSpot = spot;
  
  NOTICE("Spot: %d; current: %d", spot, _currentSpot);
  if (spot != _currentSpot) // just changed!
  {
    _CLIP_CHANGED->type()->setValue(1);
    _previousSpot = _currentSpot;
    _currentSpot = spot;
  }
  else
  {
    _CLIP_CHANGED->type()->setValue(0);
  }

  _CURRENT_CLIP->type()->setValue( (*data)[_currentSpot].clipFileNames[0] );
  _PREVIOUS_CLIP->type()->setValue( (*data)[_previousSpot].clipFileNames[0] );

  NOTICE("Current clip: %s.", _CURRENT_CLIP->type()->value().c_str());
  NOTICE("Prevous clip: %s.", _PREVIOUS_CLIP->type()->value().c_str());
}



