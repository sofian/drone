/* Gear_KDTree.h
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

#ifndef GEAR_KDTREE_INCLUDED
#define GEAR_KDTREE_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"
#include "AreaArrayType.h"
#include <assert.h>
#include "Timing.h"
#include "Rasterer.h"
#include "SummedAreaTable.h"

class Gear_KDTree : public Gear  
{

public:
  Gear_KDTree(Schema *schema, std::string uniqueName);
  virtual ~Gear_KDTree();

  void runVideo();
  bool ready();

  void init();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<AreaArrayType> *_AREA_OUT;
  PlugIn<ValueType> *_AMOUNT_IN;

  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 

  SummedAreaTable<> *_table;

  int _sizeX;
  int _sizeY;
  //  int _size;

  int _r;
  int _g;
  int _b;

  unsigned char *_imageIn;
  unsigned char *_imageOut;

  Rasterer *_rasterer;

  int _maxDepth;

  void split(int x0, int x1, int y0, int y1, int depth);
};

#endif 
