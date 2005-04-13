/* Gear_SmearGrid.cpp
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

#include "Gear_SmearGrid.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_SmearGrid(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "SmearGrid";
  gearInfo.classification = GearClassifications::video().distortion().instance();
  return gearInfo;
}
}

Gear_SmearGrid::Gear_SmearGrid(Schema *schema, std::string uniqueName) : Gear(schema, "SmearGrid", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_GRID_OUT = new PlugOut<DisplaceGrid>(this, "GRID"));
  addPlug(_PARAM1 = new PlugIn<ValueType>(this, "PARAM1", new ValueType(0, -M_PI, M_PI)));
  addPlug(_PARAM2 = new PlugIn<ValueType>(this, "PARAM2", new ValueType(0, 0, 2)));
  addPlug(_SCALE = new PlugIn<ValueType>(this, "SCALE", new ValueType(1,0,2)));
}

Gear_SmearGrid::~Gear_SmearGrid()
{
}

bool Gear_SmearGrid::ready()
{
  return(_VIDEO_IN->connected() && _GRID_OUT->connected() );
}

void Gear_SmearGrid::runVideo()
{
  _image = _VIDEO_IN->type();
  
  if (_image->isNull())
    return;

  _data = (unsigned char*)_image->data();    

  _ysize = _image->height();
  _xsize = _image->width();

  _param1 = _PARAM1->type()->value();
  _param2 = _PARAM2->type()->intValue();
  _scale = _SCALE->type()->value();

  _halfdiag = sqrt(_xsize*_xsize+_ysize*_ysize); 
  
  _gridOut = _GRID_OUT->type();
  _gridOut->resize(_xsize,_ysize);

  _gridData = _gridOut->data();
  
  float xx,yy;
  
  char idx1=0,idx2=0;
  switch(_param2)
  {
  case 0: 
    idx1=0;idx2=1;
    break;
  case 1: 
    idx1=1;idx2=2;
    break;
  case 2: 
    idx1=0;idx2=2;
    break;
  }  

  float cos_ = _scale * cos(_param1), sin_= _scale * sin(_param1);

  for (int y=0 ; y<_ysize ; y++)
  {
    for (int x=0 ; x<_xsize ; x++)
    {        
      xx = ((float)(_data[idx1])-128);
      yy = ((float)(_data[idx2])-128);
      
      _gridData->xdisp = (xx*cos_ + yy*sin_);
      _gridData->ydisp = (yy*cos_ - xx*sin_);

      _data+=4;
      _gridData++;
    }
  }   
}
