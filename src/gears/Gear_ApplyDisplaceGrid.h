/* Gear_ApplyDisplaceGrid.h
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#ifndef GEAR_APPLYDISPLACEGRID_INCLUDED
#define GEAR_APPLYDISPLACEGRID_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"
#include "DisplaceGrid.h"

class Gear_ApplyDisplaceGrid : public Gear
{
public:

  Gear_ApplyDisplaceGrid(Engine *engine, std::string name);
  virtual ~Gear_ApplyDisplaceGrid();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<DisplaceGrid> *_GRID;
  PlugIn<ValueType> *_MODE;

  //local var
  const VideoTypeRGBA *_image;     
  VideoTypeRGBA *_outImage; 
  const RGBA *_data;

  const DisplaceGrid *_grid;

  RGBA *_inData;
  RGBA *_outData;
  float * _gridData;

  int _nx,_ny;

  int _mode;

  int _imsizeY;
  int _imsizeX;

  int _gridsizeY;
  int _gridsizeX;

  void init();    
};

#endif
