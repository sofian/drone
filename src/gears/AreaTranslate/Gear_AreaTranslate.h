/* Gear_AreaTranslate.h
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#ifndef GEAR_AREATRANSLATE_INCLUDED
#define GEAR_AREATRANSLATE_INCLUDED


#include "Gear.h"
#include "ValueType.h"
#include "AreaType.h"


class Gear_AreaTranslate : public Gear
{
public:

  Gear_AreaTranslate(Schema *schema, QString uniqueName);
  virtual ~Gear_AreaTranslate();

  void runVideo();

protected:
  void internalInit();

private:
  PlugOut<AreaType> *_AREA_OUT;

  PlugIn<AreaType> *_AREA_IN;
  PlugIn<ValueType> *_X_IN;
  PlugIn<ValueType> *_Y_IN;

  const AreaType *_areaIn;
  AreaType *_areaOut;
};

#endif
