/* VideoOutput.h
 * Copyright (C) 2004 Mathieu Guindon
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

#ifndef VIDEOOUTPUT_INCLUDED
#define VIDEOOUTPUT_INCLUDED

#include "ColorSpace.h"
#include "VideoRGBAType.h"

class VideoOutput
{
public:
  VideoOutput() : _xRes(0), _yRes(0), _fullscreen(false) {}
  virtual ~VideoOutput(){}

  virtual bool init(int xRes, int yRes, bool fullscreen)=0;    
  virtual void render(const VideoRGBAType &image)=0;
  virtual void fullscreen(bool fs)=0;
  virtual void destroy()=0;

protected:
  int _xRes, _yRes;
  int _bpp;
  bool _fullscreen;

};


#endif

