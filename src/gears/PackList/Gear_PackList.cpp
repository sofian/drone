/* Gear_PackList.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
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


#include "Gear_PackList.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_PackList(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "PackList";
  gearInfo.classification = GearClassifications::protocol().osc().instance();
  return gearInfo;
}
}

Gear_PackList::Gear_PackList(Schema *schema, std::string uniqueName) : 
  Gear(schema, "PackList", uniqueName)
{

  addPlug(_STR1 = new PlugIn<StringType>(this, "Str1", true, new StringType("")));
	addPlug(_STR2 = new PlugIn<StringType>(this, "Str2", true, new StringType("")));
  addPlug(_LIST_OUT = new PlugOut<ListType>(this, "List", true));
}

Gear_PackList::~Gear_PackList()
{

}

void Gear_PackList::runVideo()
{
	ListType *listType = _LIST_OUT->type();

	listType->push_back(_STR1->type());
	listType->push_back(_STR2->type());
}


void Gear_PackList::clearList()
{
	ListType *listType = _LIST_OUT->type();
	
	
	for(ListType::iterator it=listType->begin(); it!=listType->end(); ++it)
	{
		delete (*it);
	}
	
	listType->clear();
	
}
