/* Gear_GenDisplaceGrid.h
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

#ifndef GEAR_GENDISPLACEGRID_INCLUDED
#define GEAR_GENDISPLACEGRID_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "DisplaceGrid.h"
#include "VideoRGBAType.h"

class Gear_GenDisplaceGrid : public Gear
{
public:

  Gear_GenDisplaceGrid(Schema *schema, std::string uniqueName);
  virtual ~Gear_GenDisplaceGrid();

  void runVideo();

private:

  PlugOut<DisplaceGrid> *_GRID_OUT;
  PlugIn<ValueType> *_TYPE;
  PlugIn<ValueType> *_PARAM1;
  PlugIn<ValueType> *_PARAM2;
  PlugIn<ValueType> *_PARAM3;
  PlugIn<ValueType> *_SCALE;
  PlugIn<VideoRGBAType> *_VIDEO_IN;


  //local vars
  float _param1,_param2,_param3,_scale;
  float _lastparam1,_lastparam2,_lastparam3,_lasttype,_lastscale;

  int _xsize,_ysize,_type;
  //!image halfdiagonal
  float _halfdiag;

  XYDisp *_gridData; 
  DisplaceGrid * _gridOut;
  const VideoRGBAType *_image;     

  unsigned char* _data; // pointer on input image

};

#endif
