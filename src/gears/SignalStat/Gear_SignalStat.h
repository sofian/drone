/* Gear_SignalStat.h
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

#ifndef GEAR_SIGNALSTAT_INCLUDED
#define GEAR_SIGNALSTAT_INCLUDED

#include "GearConverter.h"
#include "types/SignalType.h"
#include "types/ValueType.h"

class Gear_SignalStat : public GearConverter<SignalType, ValueType>
{
public:
  Gear_SignalStat(Schema *schema, QString uniqueName);
  virtual ~Gear_SignalStat() {}

  void convert();

protected:
  PlugIn<ValueType> *_FUNC;
};


#endif
