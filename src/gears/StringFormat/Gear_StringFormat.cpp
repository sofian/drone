/* Gear_StringFormat.cpp
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


#include "Gear_StringFormat.h"
#include "Engine.h"



#include <stdio.h>

extern "C" {
Gear* makeGear()
{
  return new Gear_StringFormat();
}


}

Gear_StringFormat::Gear_StringFormat() : 
  Gear("StringFormat")
{
  addPlug(_FORMAT = new PlugIn<StringType>(this, "Format", true));
  addPlug(_PARAMS = new PlugIn<ListType>(this, "Params", false));
  
  addPlug(_STRING = new PlugOut<StringType>(this, "String", true));
}

Gear_StringFormat::~Gear_StringFormat()
{

}

void Gear_StringFormat::runVideo()
{
  const ListType *listType = _PARAMS->type();
  QString format = _FORMAT->type()->value();
  
  QString outputString = "";

  for (ListType::const_iterator it = listType->begin(); it != listType->end(); ++it)
  {
    int pos1, pos2;
    if ((*it)->typeName() == StringType().typeName())
    {
      pos1 = format.find('%');
      pos2 = format.find('s', pos1);
      StringType *type = (StringType*)*it;
      sprintf(_buffer, format.ascii(), type->value().ascii());
    }
    else if ((*it)->typeName() == ValueType().typeName())
    {
      pos1 = format.find('%');
      pos2 = format.find('d', pos1);
      ValueType *type = (ValueType*)*it;
      sprintf(_buffer, format.ascii(), type->intValue());
    }

    outputString += _buffer;
    format = format.right(pos2+1);

    NOTICE("Outputstring: %s, format: %s.", outputString.ascii(), format.ascii());
  }

  outputString += format;
  
  _STRING->type()->setValue(outputString);
}

