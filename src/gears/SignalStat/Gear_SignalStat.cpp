/* Gear_SignalStat.cpp
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

#include "Gear_SignalStat.h"
#include "DroneMath.h"



extern "C" {
Gear* makeGear()
{
  return new Gear_SignalStat();
}


}

Gear_SignalStat::Gear_SignalStat()
  : GearConverter<SignalType, ValueType>("SignalStat")
{
  addPlug(_FUNC = new PlugIn<ValueType>(this, "Func", false, new ValueType(0, 0, 6)));

}

void Gear_SignalStat::convert()
{
  int size = _PLUG_IN->type()->size();

  switch((int)_FUNC->type()->value())
  {
  //first of buffer
  case 0:
      _PLUG_OUT->type()->setValue( _PLUG_IN->type()->data()[0]);
      break;
  //average
  case 1:
    //   NOTICE("%f",123.123);// (float)ssize);
      _PLUG_OUT->type()->setValue( sum(_PLUG_IN->type()->data(), size) / size );
      break;
  // min
  case 2:
      _PLUG_OUT->type()->setValue( min(_PLUG_IN->type()->data(), size));
      break;
  // max
  case 3:
      _PLUG_OUT->type()->setValue( max(_PLUG_IN->type()->data(), size));
      break;
  }

}
