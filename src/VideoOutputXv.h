/* VideoOutputXv.h
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

#ifndef VIDEOOUTPUTXV_INCLUDED
#define VIDEOOUTPUTXV_INCLUDED

#include "VideoOutputX11Base.h"

namespace X11
{

extern "C"{
#include <X11/extensions/Xvlib.h>
}

class Canvas;

class VideoOutputXv : public VideoOutputX11Base
{
public:
  VideoOutputXv();
  ~VideoOutputXv();

  void fullscreen(bool fs);
  void destroy();
  bool init(int xRes, int yRes, bool fullscreen);
  void render(const Array2DType<RGBA> &image);

protected:
  struct XvImageFormatValuesPortPair
  {
    XvImageFormatValues format;
    XvPortID port;
  } ;

  bool checkXvExtension();
  bool selectXvPort();
  void showFormat(const XvImageFormatValues &format);
  void unGrabXvPort();
  XvImage* createXvImage(int sizeX, int sizeY);
  void destroyXvImage();

  

private:

  XvImageFormatValuesPortPair _selectedFormat;

  XvImage* _xvImage;
  XShmSegmentInfo *_shmInfo;
  int _frameSizeX;
  int _frameSizeY;
  int _frameSize;

};
}

#endif
