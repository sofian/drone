/* Gear_UnpackList.cpp
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


#include "Gear_UnpackList.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_UnpackList(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "UnpackList";
  gearInfo.classification = GearClassifications::protocol().osc().instance();
  return gearInfo;
}
}

Gear_UnpackList::Gear_UnpackList(Schema *schema, std::string uniqueName) : 
  Gear(schema, "UnpackList", uniqueName)
{
  addPlug(_LIST_IN = new PlugIn<ListType>(this, "ListI", false));

  addPlug(_STR_OUT = new PlugOut<StringType>(this, "StrO", false));
  addPlug(_VAL_OUT = new PlugOut<ValueType>(this, "StrO", false));
  addPlug(_ENUM_OUT = new PlugOut<EnumType>(this, "EnumO", false));
  addPlug(_CHANNEL_OUT = new PlugOut<VideoChannelType>(this, "ChannelO", false));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "VideoO", false));
  addPlug(_AREA_OUT = new PlugOut<AreaType>(this, "AreaO", false));
  
  std::vector<AbstractPlug*> atLeastOneOfThem;
  atLeastOneOfThem.push_back(_STR_OUT);
  atLeastOneOfThem.push_back(_VAL_OUT);
  atLeastOneOfThem.push_back(_ENUM_OUT);
  atLeastOneOfThem.push_back(_CHANNEL_OUT);
  atLeastOneOfThem.push_back(_VIDEO_OUT);
  atLeastOneOfThem.push_back(_AREA_OUT);
  setPlugAtLeastOneNeeded(atLeastOneOfThem);

  addPlug(_LIST_OUT = new PlugOut<ListType>(this, "ListO", true));
}

Gear_UnpackList::~Gear_UnpackList()
{

}

void Gear_UnpackList::runVideo()
{
  ListType *listType = _LIST_OUT->type();
  
  if (_LIST_IN->connected())
    listType->assign(_LIST_IN->type()->begin(), _LIST_IN->type()->end());
  else 
    clearList();  

  AbstractType *type = listType->pop_back();
  if (type->typeName() == StringType::TYPENAME)
    (*_STR_OUT) = *((StringType*)type);
}


void Gear_UnpackList::clearList()
{
	ListType *listType = _LIST_OUT->type();
	
	listType->clear();
	
}

void Gear_UnpackList::internalPrePlay()
{
  clearList();
}  
