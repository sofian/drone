/* VideoOutputGl.h
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

#ifndef VIDEOOUTPUTGL_INCLUDED
#define VIDEOOUTPUTGL_INCLUDED

#include "VideoOutputX11Base.h"

namespace X11
{

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <X11/extensions/xf86vmode.h>

class Canvas;

class VideoOutputGl : public VideoOutputX11Base
{
public:
  VideoOutputGl();
  ~VideoOutputGl();

  void fullscreen(bool fs);
  void destroy();
  bool init(int xRes, int yRes, bool fullscreen);
  void render(const Array2DType<RGBA> &image);


protected:

  void onResize(int sizeX, int sizeY);
  bool createGLXContext();               
  int initGl(int xRes, int yRes);
  void resizeGl(int sizeX, int sizeY);
  void destroyGLXContext();


private:

  GLXContext _XGLXContext;
  int _frameSizeX;
  int _frameSizeY;
  int _frameSize;
  bool _glInitialized;
  float _texSizeX, _texSizeY;

};
}

#endif
