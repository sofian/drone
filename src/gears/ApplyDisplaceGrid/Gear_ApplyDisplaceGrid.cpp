/* Gear_ApplyDisplaceGrid.cpp
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

#include "Gear_ApplyDisplaceGrid.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Engine *engine, std::string name)
{
  return new Gear_ApplyDisplaceGrid(engine,name);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "ApplyDisplaceGrid";
  gearInfo.classification = GearClassifications::video().distortion().instance();
  return gearInfo;
}
}

Gear_ApplyDisplaceGrid::Gear_ApplyDisplaceGrid(Engine *engine, std::string name) : Gear(engine, "ApplyDisplaceGrid", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  addPlug(_GRID = new PlugIn<DisplaceGrid>(this, "Grid"));
  addPlug(_MODE = new PlugIn<ValueType>(this, "Mode", new ValueType(0.0f,0.0f,1.0f)));
}

Gear_ApplyDisplaceGrid::~Gear_ApplyDisplaceGrid()
{
}

void Gear_ApplyDisplaceGrid::init()
{
}

bool Gear_ApplyDisplaceGrid::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_ApplyDisplaceGrid::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _grid = _GRID->type();
  _outImage = _VIDEO_OUT->type();

  _imsizeY = _image->height();
  _imsizeX = _image->width();

  _gridsizeY = _grid->height();
  _gridsizeX = _grid->width();

  _outImage->resize(_imsizeX, _imsizeY);


  if(_imsizeX != _gridsizeX || _imsizeY != _gridsizeY)
  {
    WARNING("applyDisplaceGrid : gridsize(%i,%i) != imageSize(%i,%i).. resizing not supported yet",_gridsizeX,_gridsizeY,_imsizeX,_imsizeY);
    return;
  }
  
  ////////////////////////////
  
  _gridData = (float*)(_GRID->type()->data());
  _data = _image->data();    
  _outData = _outImage->data();
  
  _mode = (int)_MODE->type()->intValue();

  if(_mode==0)
  {
    for (int y=0;y<_imsizeY;y++)
    {
      for (int x=0;x<_imsizeX;x++)
      {
        _nx = (int)(x + *_gridData++);
        _ny = (int)(y + *_gridData++);

        _nx=MIRROR_CLAMP(_nx,0,_imsizeX-1);
        _ny=MIRROR_CLAMP(_ny,0,_imsizeY-1);

        ASSERT_ERROR(_nx>=0 && _nx<=_imsizeX-1);
        ASSERT_ERROR(_ny>=0 && _ny<=_imsizeY-1);      

        *_outData = *(_image->row(_ny)+_nx);

        //NOTICE("data %i _outData %i _nx %i _ny %i x %i y %i ", _data, _outData, _nx-x, _ny-y, x, y);
        //memcpy(_outData,_data,sizeof(RGBA));
        _outData++;//=4;
      }
    }   
  }
  else
  {
    _outImage->fill(BLACK_RGBA);
    for (int y=0;y<_imsizeY;y++)
    {
      for (int x=0;x<_imsizeX;x++)
      {
        _nx = (int)(x + *_gridData++);
        _ny = (int)(y + *_gridData++);

        _nx=MIRROR_CLAMP(_nx,0,_imsizeX-1);
        _ny=MIRROR_CLAMP(_ny,0,_imsizeY-1);

        ASSERT_ERROR(_nx>=0 && _nx<=_imsizeX-1);
        ASSERT_ERROR(_ny>=0 && _ny<=_imsizeY-1);      

        *(_outImage->row(_ny)+_nx) = *_data;

        //NOTICE("data %i _outData %i _nx %i _ny %i x %i y %i ", _data, _outData, _nx-x, _ny-y, x, y);
        //memcpy(_outData,_data,sizeof(RGBA));
        _data++;//=4;
      }
    }   

  }
    
  

}
