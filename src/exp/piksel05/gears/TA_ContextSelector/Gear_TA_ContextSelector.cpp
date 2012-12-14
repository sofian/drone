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
#include "DroneMath.h"

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

const int Gear_TA_ContextSelector::MAX_CONTEXTS = 2;

Gear_TA_ContextSelector::Gear_TA_ContextSelector(Schema *schema, std::string uniqueName) : 
  Gear(schema, "TA_ContextSelector", uniqueName),
  _currentContext(0)
{
  addPlug(_INNOCENCE_IN = new PlugIn<ValueType>(this, "Ict", false));
  addPlug(_CHANNEL_IN = new PlugIn<ValueType>(this, "Ch", false));
  addPlug(_BANG_IN = new PlugIn<ValueType>(this, "Bang", false));

//   _INNOCENCE_OUT.resize(MAX_CONTEXTS);
//   _CHANNEL_OUT.resize(MAX_CONTEXTS);
//   _BANG_OUT.resize(MAX_CONTEXTS);
//   for (int i=0; i<MAX_CONTEXTS; ++i)
//   {
    addPlug(_INNOCENCE_0_OUT = new PlugOut<ValueType>(this, "Ict0", false));    
    addPlug(_CHANNEL_0_OUT = new PlugOut<ValueType>(this, "Ch0", false));
    addPlug(_BANG_0_OUT = new PlugOut<ValueType>(this, "Bang0", false));
    addPlug(_INNOCENCE_1_OUT = new PlugOut<ValueType>(this, "Ict1", false));    
    addPlug(_CHANNEL_1_OUT = new PlugOut<ValueType>(this, "Ch1", false));
    addPlug(_BANG_1_OUT = new PlugOut<ValueType>(this, "Bang1", false));
//     addPlug(new PlugOut<ValueType>(this, "Ict", false));
//     addPlug(new PlugOut<ValueType>(this, "Ch", false));
//     addPlug(new PlugOut<ValueType>(this, "Bang", false));
//  }
}

Gear_TA_ContextSelector::~Gear_TA_ContextSelector()
{
}

void Gear_TA_ContextSelector::runVideo()
{
  _BANG_0_OUT->type()->setValue(0);
  _BANG_1_OUT->type()->setValue(0);
  
  if (_BANG_IN->type()->boolValue())
  {
    if (_currentContext == 0)
    {
      _INNOCENCE_0_OUT->type()->setValue(_INNOCENCE_IN->type()->value());
      _CHANNEL_0_OUT->type()->setValue(_CHANNEL_IN->type()->value());
      _BANG_0_OUT->type()->setValue(_BANG_IN->type()->value());
    }
    else if (_currentContext == 1)
    {
      _INNOCENCE_1_OUT->type()->setValue(_INNOCENCE_IN->type()->value());
      _CHANNEL_1_OUT->type()->setValue(_CHANNEL_IN->type()->value());
      _BANG_1_OUT->type()->setValue(_BANG_IN->type()->value());
    }
    std::cout << _currentContext << std::endl;
    _currentContext = ( _currentContext + 1 ) % MAX_CONTEXTS;
  }
}



