/* Gear_GenDisplaceGrid.cpp
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

#include "Gear_GenDisplaceGrid.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_GenDisplaceGrid, Gear_GenDisplaceGrid, "GenDisplaceGrid")

Gear_GenDisplaceGrid::Gear_GenDisplaceGrid(Engine *engine, std::string name) : Gear(engine, "GenDisplaceGrid", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_GRID_OUT = new PlugOut<DisplaceGrid>(this, "GRID"));
  addPlug(_TYPE = new PlugIn<ValueType>(this, "TYPE", new ValueType(0,0,1)));
  addPlug(_PARAM1 = new PlugIn<ValueType>(this, "PARAM1", new ValueType(3, 0, 50)));
  addPlug(_PARAM2 = new PlugIn<ValueType>(this, "PARAM2", new ValueType(0, -M_PI, M_PI)));
  addPlug(_PARAM3 = new PlugIn<ValueType>(this, "PARAM3", new ValueType(2*M_PI, 0, 2*M_PI)));
  addPlug(_SCALE = new PlugIn<ValueType>(this, "SCALE", new ValueType(1,0,2)));

  // hum.. ugly
  _lastparam1=_param1+1;
  _lastparam2=_param2+1;
  _lastparam3=_param3+1;
  _lasttype=_type+1;
  _lastscale=_scale+1;
}

Gear_GenDisplaceGrid::~Gear_GenDisplaceGrid()
{
}

void Gear_GenDisplaceGrid::init()
{
}

bool Gear_GenDisplaceGrid::ready()
{
  return(_GRID_OUT->connected() );
}

void Gear_GenDisplaceGrid::runVideo()
{
  _type = _TYPE->type()->intValue();
  _image = _VIDEO_IN->type();
  _data = (unsigned char*)_image->data();    

  _ysize = _image->height();
  _xsize = _image->width();

  _param1 = _PARAM1->type()->value();
  _param2 = _PARAM2->type()->value();
  _param3 = _PARAM3->type()->value();
  _scale = _SCALE->type()->value();

  // we should have a generic machanism for this..
  if(_type!=0 && _param1==_lastparam1 && _param2==_lastparam2 && _param3==_lastparam3 && _type==_lasttype && _scale==_lastscale)
    return;
  _lastparam1=_param1;
  _lastparam2=_param2;
  _lastparam3=_param3;
  _lasttype = _type;
  _lastscale = _scale;
  _halfdiag = sqrt(_xsize*_xsize+_ysize*_ysize); 
  
  _gridOut = _GRID_OUT->type();
  _gridOut->resize(_xsize,_ysize);

  _gridData = _gridOut->data();
  
  float fx,fy,theta,rho;

  fy = - _ysize/2;
  
  switch(_type)
  {
  case 0:
    for (int y=0 ; y<_ysize ; y++)
    {
      for (int x=0 ; x<_xsize ; x++)
      {        
        _gridData->xdisp = _scale * ((float)(*_data++)-128);
        _gridData->ydisp = _scale * ((float)(*_data++)-128);
        _data+=2;
        _gridData++;
      }
    }   

  break;

  case 1:
    for (int y=0 ; y<_ysize ; y++)
    {
      fx = - _xsize/2;
      
      for (int x=0 ; x<_xsize ; x++)
      {
        rho = fastDist(fx,fy);
        theta = MIRROR_CLAMP(fastAngle(fx,fy),_param2,_param2+_param1);
        
        rho=MIRROR_CLAMP(rho,0.0f,_param3);
        
//rho /= -_halfdiag;
        //NOTICE("gd %i rho %f theta %f x %i y %i xsize %i ysize %i",_gridData, rho,theta, x,y,_xsize,_ysize);
        
        _gridData->xdisp = cos(theta)*rho;//fastPolarY(rho,theta);;
        _gridData->ydisp = sin(theta)*rho;//fastPolarY(rho,theta);;
        _data+=2;
        _gridData++;
      }
      
    }   

  break;
  // waves
 case 2:

    for (float y=0 ; y<_ysize ; y++)
    {
      for (float x=0 ; x<_xsize ; x++)
      {
        _gridData->xdisp = _scale* cos(x/_xsize*_param1)*_param2;//fastPolarX(rho,theta);
        _gridData->ydisp = _scale* sin(y/_ysize*_param1)*_param2;//fastPolarY(rho,theta);
        _gridData++;
        
      }
    }   
    break;



  }
}
