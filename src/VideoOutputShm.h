/* VideoOutputShm.h
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

#ifndef VIDEOOUTPUTSHM_INCLUDED
#define VIDEOOUTPUTSHM_INCLUDED

#include "VideoOutputX11Base.h"

namespace X11
{

class Canvas;

class VideoOutputShm : public VideoOutputX11Base
{
public:
  VideoOutputShm();
  ~VideoOutputShm();

  void fullscreen(bool fs);
  void destroy();
  bool init(int xRes, int yRes, bool fullscreen);
  void render(const MatrixType<RGBA> &image);

protected:


  XImage* createXImage(int sizeX, int sizeY);
  void destroyXImage();
  void onResize(int sizeX, int sizeY);

private:


  XImage* _xImage;
  int _frameSizeX;
  int _frameSizeY;
  int _frameSize;

};
}
#endif
