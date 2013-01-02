/* VideoChannelType.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef VIDEOCHANNELTYPE_INCLUDED
#define VIDEOCHANNELTYPE_INCLUDED

#include "ColorSpace.h"
#include "Array2DType.h"

class VideoChannelType : public Array2DType<unsigned char>
{
public:
  VideoChannelType(int width = 0,
                   int height = 0,
                   unsigned char fillValue = 0)
    : Array2DType<unsigned char>(width, height, fillValue)
  {
  }

  virtual ~VideoChannelType() {}

  TYPE_BASE_METHODS(VideoChannelType, (63, 63, 63))
};

#endif //VIDEOCHANNELTYPE_INCLUDED
