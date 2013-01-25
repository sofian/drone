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



extern "C" {
Gear* makeGear()
{
  return new Gear_PackList();
}


}

Gear_PackList::Gear_PackList() : 
  Gear("PackList")
{

  addPlug(_ELEM_IN = new PlugIn<AbstractType>(this, "Elem", true));
  addPlug(_LIST_IN = new PlugIn<ListType>(this, "ListI", false));
  addPlug(_LIST_OUT = new PlugOut<ListType>(this, "ListO", true));
}

Gear_PackList::~Gear_PackList()
{
  clearList();
}

void Gear_PackList::runVideo()
{
  ListType *listType = _LIST_OUT->type();
  
  // XXX Maybe there's a more efficient way than clearing/populating each time but this
  // is safe memory-wise.
  clearList();

  if (_LIST_IN->connected()) {
    const ListType* inputList = _LIST_IN->type();
    for(ListType::const_iterator it=inputList->begin(); it!=inputList->end(); ++it)
      listType->push_back( (*it)->clone() );
  }

  listType->push_back(_ELEM_IN->cloneType());
}


void Gear_PackList::clearList()
{
	ListType *listType = _LIST_OUT->type();
	for(ListType::iterator it=listType->begin(); it!=listType->end(); ++it)
	  delete (*it);
	listType->clear();
}

void Gear_PackList::internalPrePlay()
{
  clearList();
}  
