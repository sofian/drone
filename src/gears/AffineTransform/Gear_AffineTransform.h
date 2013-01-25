/* Gear_AffineTransform.h
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

#ifndef GEAR_AFFINETRANSFORM_INCLUDED
#define GEAR_AFFINETRANSFORM_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "ValueType.h"

class PlugSignal;

class Gear_AffineTransform : public Gear  
{
public:
  Gear_AffineTransform();
  virtual ~Gear_AffineTransform();

  void runVideo();
  void runAudio();

private:
  PlugIn<ValueType> *_VALUE_IN,*_SCALE,*_OFFSET;
  PlugOut<ValueType> *_VALUE_OUT;
};

#endif 
