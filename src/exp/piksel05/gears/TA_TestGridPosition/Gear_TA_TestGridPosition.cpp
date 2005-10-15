/* Gear_TA_TestGridPosition.cpp
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
#include "Gear_TA_TestGridPosition.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_TestGridPosition(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_TestGridPosition";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

Gear_TA_TestGridPosition::Gear_TA_TestGridPosition(Schema *schema, std::string uniqueName) : 
Gear(schema, "TA_TestGridPosition", uniqueName)
{
  addPlug(_X0_IN = new PlugIn<ValueType>(this, "X0", false, new ValueType(-0.5, -0.5, 1.5)));
  addPlug(_Y0_IN = new PlugIn<ValueType>(this, "Y0", false, new ValueType(-0.5, -0.5, 1.5)));
  addPlug(_X1_IN = new PlugIn<ValueType>(this, "X1", false, new ValueType(-0.5, -0.5, 1.5)));
  addPlug(_Y1_IN = new PlugIn<ValueType>(this, "Y1", false, new ValueType(-0.5, -0.5, 1.5)));

  addPlug(_GRID_OUT = new PlugOut<VideoChannelType>(this, "DataOut", false));
  addPlug(_RGBA_GRID_OUT = new PlugOut<VideoRGBAType>(this, "RGBAOut", false));

  std::vector<AbstractPlug*> atLeastOneOfThem;
  atLeastOneOfThem.push_back(_GRID_OUT);
  atLeastOneOfThem.push_back(_RGBA_GRID_OUT);
  setPlugAtLeastOneNeeded(atLeastOneOfThem);
}

Gear_TA_TestGridPosition::~Gear_TA_TestGridPosition()
{
}

void Gear_TA_TestGridPosition::runVideo()
{
  float x0 = _X0_IN->type()->value();
  float y0 = _Y0_IN->type()->value();
  float x1 = _X1_IN->type()->value();
  float y1 = _Y1_IN->type()->value();

  Array2D<unsigned char> *out = _GRID_OUT->type();
  out->resize(TA_GRID_WIDTH, TA_GRID_HEIGHT);
  out->fill(0);

  if (0 <= x0 && x0 <= 1 && 0 <= y0 && y0 <= 1) // Cire's on the carpet : send the *** magic ***!
    (*out)( (int)(x0*(float)TA_GRID_WIDTH), (int)(y0*(float)TA_GRID_HEIGHT)) = 0xff;

  if (0 <= x1 && x1 <= 1 && 0 <= y1 && y1 <= 1) // Gry's on the carpet : send the *** magic (bis) ***!
    (*out)( (int)(x1*(float)TA_GRID_WIDTH), (int)(y1*(float)TA_GRID_HEIGHT)) = 0xff;

  if (_RGBA_GRID_OUT->connected())
  {
    Array2D<RGBA> *rgbaOut = _RGBA_GRID_OUT->type();
    rgbaOut->resize(out->width(), out->height());
    for (int i=0; i<(int)out->size(); ++i)
    {
      unsigned char val = (*out)[i];
      (*rgbaOut)[i] = RGBA(val, val, val, 255);
    }
  }
}



