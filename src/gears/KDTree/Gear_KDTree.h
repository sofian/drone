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

protected:  
  void internalInit();

private:

  // Inputs.
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugIn<ValueType> *_DEPTH_IN;
  PlugIn<ValueType> *_H_FIRST_IN;
  PlugIn<ValueType> *_H_CELLS_IN;
  PlugIn<ValueType> *_V_CELLS_IN;
  PlugIn<ValueType> *_MIN_CELLSIZE_IN;

  // Outputs.
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<AreaArrayType> *_AREA_OUT;

  // Internal use.
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 

  SummedAreaTable<> *_table;
  //  SummedAreaTable<unsigned char, int, 1> *_intensitiesTable;
  //  Array2D<unsigned char> _intensities;

  int _sizeX;
  int _sizeY;

  int _r;
  int _g;
  int _b;

  unsigned char *_imageIn;
  unsigned char *_imageOut;

  Rasterer *_rasterer;

  int _maxDepth;
  int _minCellSize; // minimum width and/or height of a cell
  int _nHCells, _nVCells;
  int _cuts[1024];

  void split(int x0, int x1, int y0, int y1, int depth, bool hSplit);
  static inline unsigned int intensity(unsigned int r, unsigned int g, unsigned int b);
};

#include <iostream>
// XXX ça devrait aller à une seule place
inline unsigned int Gear_KDTree::intensity(unsigned int r, unsigned int g, unsigned int b)
{
  return (r + (g>>1) + b)  >> 2;
}

#endif 
