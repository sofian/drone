/* Gear_TA_ContextSelector.cpp
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
#include "Gear_TA_ContextSelector.h"
#include "Engine.h"
#include "Math.h"

#include "GearMaker.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_ContextSelector(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_ContextSelector";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

const int Gear_TA_ContextSelector::MAX_CONTEXTS = 5;

Gear_TA_ContextSelector::Gear_TA_ContextSelector(Schema *schema, std::string uniqueName) : 
  Gear(schema, "TA_ContextSelector", uniqueName),
  _currentContext(0)
{
  addPlug(_INNOCENCE_IN = new PlugIn<ValueType>(this, "Ict", false));
  addPlug(_CHANNEL_IN = new PlugIn<ValueType>(this, "Ch", false));
  addPlug(_BANG_IN = new PlugIn<ValueType>(this, "Bang", false));

  for (int i=0; i<MAX_CONTEXTS; ++i)
  {
    addPlug(_INNOCENCE_OUT[i] = new PlugOut<ValueType>(this, "Ict" + i, false));
    addPlug(_CHANNEL_OUT[i] = new PlugOut<ValueType>(this, "Ch" + i, false));
    addPlug(_BANG_OUT[i] = new PlugOut<ValueType>(this, "Bang" + i, false));
  }
}

Gear_TA_ContextSelector::~Gear_TA_ContextSelector()
{
}

void Gear_TA_ContextSelector::runVideo()
{
  if (_BANG_IN->type()->boolValue())
  {
    _INNOCENCE_OUT[_currentContext]->type()->setValue(_INNOCENCE_IN->type()->value());
    _CHANNEL_OUT[_currentContext]->type()->setValue(_CHANNEL_IN->type()->value());
    _BANG_OUT[_currentContext]->type()->setValue(_BANG_IN->type()->value());
  }
}



