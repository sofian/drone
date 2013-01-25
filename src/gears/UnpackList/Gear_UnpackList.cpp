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



extern "C" {
Gear* makeGear()
{
  return new Gear_UnpackList();
}


}

Gear_UnpackList::Gear_UnpackList() : 
  Gear("UnpackList")
{
  addPlug(_LIST_IN = new PlugIn<ListType>(this, "ListI", true));

  addPlug(_STR_OUT = new PlugOut<StringType>(this, "StrO", false));
  addPlug(_VAL_OUT = new PlugOut<ValueType>(this, "ValueO", false));
  addPlug(_ENUM_OUT = new PlugOut<EnumType>(this, "EnumO", false));
  addPlug(_CHANNEL_OUT = new PlugOut<VideoChannelType>(this, "ChannelO", false));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "VideoO", false));
  addPlug(_AREA_OUT = new PlugOut<AreaType>(this, "AreaO", false));
  
  QList<AbstractPlug*> atLeastOneOfThem;
  atLeastOneOfThem.push_back(_STR_OUT);
  atLeastOneOfThem.push_back(_VAL_OUT);
  atLeastOneOfThem.push_back(_ENUM_OUT);
  atLeastOneOfThem.push_back(_CHANNEL_OUT);
  atLeastOneOfThem.push_back(_VIDEO_OUT);
  atLeastOneOfThem.push_back(_AREA_OUT);
  setPlugAtLeastOneNeeded(atLeastOneOfThem);

  addPlug(_LIST_OUT = new PlugOut<ListType>(this, "ListO", false));
}

Gear_UnpackList::~Gear_UnpackList()
{
  clearList();
}

void Gear_UnpackList::runVideo()
{
  //std::cout << name() << "::: size:: " << _LIST_IN->type()->size() << std::endl;
  if (!_LIST_IN->connected() ||
      _LIST_IN->type()->empty())
  {    
    _LIST_OUT->sleeping(true); // TODO: the sleeping mechanism just doesn't work
    return;
  }

  _LIST_OUT->sleeping(false);

  ListType *listType = _LIST_OUT->type();
  const AbstractType *type = _LIST_IN->type()->front();

  // XXX Maybe there's a more efficient way than clearing/populating each time but this
  // is safe memory-wise.
  clearList();

  // Copy
  const ListType* inputList = _LIST_IN->type();
  for(ListType::const_iterator it=inputList->begin() + 1; it!=inputList->end(); ++it)
    listType->push_back( (*it)->clone() );

  //std::cout << "got something " << type->typeName() << std::endl;
  //for (ListType::const_iterator it = _LIST_IN->type()->begin(); it != _LIST_IN->type()->end(); ++it)
  //listType->assign(_LIST_IN->type()->begin(), _LIST_IN->type()->end());
  
  if ((_STR_OUT->connected()) && type->typeName() == StringType().typeName())
  {
    //std::cout << "sending str out: " << ((StringType*)type)->value() << std::endl;
    _STR_OUT->type()->copyFrom(*type);
    return;
  }
    
  else if ((_VAL_OUT->connected()) && type->typeName() == ValueType().typeName())
  {
    //std::cout << "sending val out: " << ((ValueType*)type)->value() << std::endl;
    _VAL_OUT->type()->copyFrom(*type);
    return;
  }

  else if ((_ENUM_OUT->connected()) && type->typeName() == EnumType().typeName())
  {
    //std::cout << "sending val out: " << ((ValueType*)type)->value() << std::endl;
    _ENUM_OUT->type()->copyFrom(*type);
    return;
  }

  else if ((_CHANNEL_OUT->connected()) && type->typeName() == VideoChannelType().typeName())
  {
    //std::cout << "sending val out: " << ((ValueType*)type)->value() << std::endl;
    _CHANNEL_OUT->type()->copyFrom(*type);
    return;
  }


  else if ((_VIDEO_OUT->connected()) && type->typeName() == VideoRGBAType().typeName())
  {
    //std::cout << "sending val out: " << ((ValueType*)type)->value() << std::endl;
    _VIDEO_OUT->type()->copyFrom(*type);
    return;
  }

  else if ((_AREA_OUT->connected()) && type->typeName() == AreaType().typeName())
  {
    //std::cout << "sending val out: " << ((ValueType*)type)->value() << std::endl;
    _AREA_OUT->type()->copyFrom(*type);
    return;
  }
}


void Gear_UnpackList::clearList()
{
  ListType *listType = _LIST_OUT->type();
  for(ListType::iterator it=listType->begin(); it!=listType->end(); ++it)
    delete (*it);
  listType->clear();
}

void Gear_UnpackList::internalPrePlay()
{
  clearList();
}  
