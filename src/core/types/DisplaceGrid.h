/* DisplaceGrid.h
 * Copyright (C) 2004 Julien Keable
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

#ifndef DISPLACEGRID_INCLUDED
#define DISPLACEGRID_INCLUDED

#include "ColorSpace.h"
#include "Array2DType.h"

struct XYDisp
{
  float xdisp,ydisp;
};

const XYDisp XYDisp_STILL = {0.0f, 0.0f};

class DisplaceGrid : public Array2DType<XYDisp>
{
public:
  DisplaceGrid(int width = 0, int height = 0, XYDisp fillValue = XYDisp_STILL) : 
    Array2DType<XYDisp>(width, height, fillValue)
  {
  }

  virtual ~DisplaceGrid() {}
  
  TYPE_BASE_METHODS(DisplaceGrid, (240, 255, 14))
};

#endif //VIDEOTYPE_INCLUDED
