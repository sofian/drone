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
Gear* makeGear()
{
  return new Gear_PackList();
}


}

Gear_PackList::Gear_PackList() : 
  Gear("PackList")
{

  addPlug(_STR1 = new PlugIn<AbstractType>(this, "Str1", true));
  addPlug(_LIST_IN = new PlugIn<ListType>(this, "ListI", false));
  addPlug(_LIST_OUT = new PlugOut<ListType>(this, "ListO", true));
}

Gear_PackList::~Gear_PackList()
{

}

void Gear_PackList::runVideo()
{
  ListType *listType = _LIST_OUT->type();
  
  if (_LIST_IN->connected())
    listType->assign(_LIST_IN->type()->begin(), _LIST_IN->type()->end());
  else 
    clearList();  

  listType->push_back(_STR1->type());

}


void Gear_PackList::clearList()
{
	ListType *listType = _LIST_OUT->type();
	
/*	
	for(ListType::iterator it=listType->begin(); it!=listType->end(); ++it)
	{
	  delete (*it);
	}
*/	
	listType->clear();
	
}

void Gear_PackList::internalPrePlay()
{
  clearList();
}  
