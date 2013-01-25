/* Gear_AreaClip.cpp
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

#include <iostream>
#include "Gear_AreaClip.h"
#include "Engine.h"
#include "DroneMath.h"




extern "C" {
Gear* makeGear()
{
  return new Gear_AreaClip();
}


}

Gear_AreaClip::Gear_AreaClip() : Gear("AreaClip")
{    
  addPlug(_AREA_OUT = new PlugOut<AreaType>(this, "AreaOut", true));
  addPlug(_AREA_IN = new PlugIn<AreaType>(this, "AreaIn", true));
  addPlug(_CLIPPING_IN = new PlugIn<AreaType>(this, "Clip", true));
  addPlug(_BORDER_IN = new PlugIn<ValueType>(this, "Border", false, new ValueType(0, 0, 10)));
}

Gear_AreaClip::~Gear_AreaClip()
{
}

void Gear_AreaClip::runVideo()
{
  _areaIn = _AREA_IN->type();
  _clippingIn = _CLIPPING_IN->type();

  _areaOut = _AREA_OUT->type();
    
  _borderX = MAX(0, MIN(_BORDER_IN->type()->intValue(), (int)_areaIn->width()));
  _borderY = MAX(0, MIN(_BORDER_IN->type()->intValue(), (int)_areaIn->height()));
  
  _areaOut->setOrigin(MAX(_areaIn->x0(), _borderX), MAX(_areaIn->y0(), _borderY));
  _areaOut->resize(MIN(_areaIn->width(), (size_t)MAX(0, (int)(_clippingIn->width() - _areaIn->x0() - 2*_borderX))),
                   MIN(_areaIn->height(), (size_t)MAX(0, (int)(_clippingIn->height() - _areaIn->y0() - 2*_borderY))));
  //  NOTICE("Area out: %d %d %d %d", _areaOut->x0(), _areaOut->y0(),_areaOut->width(), _areaOut->height());  
}


