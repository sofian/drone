/* VideoOutputShm.cpp
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

#include "VideoOutputShm.h"
#include "VideoOutputMaker.h"

#include <iostream>

using namespace X11;

Register_VideoOutput(MAKERVideoOutputShm, VideoOutputShm, "Shm")

VideoOutputShm::VideoOutputShm() :
VideoOutputX11Base(),
_xImage(NULL),
_frameSizeX(0),
_frameSizeY(0),
_frameSize(0)
{    
}

VideoOutputShm::~VideoOutputShm()
{
  destroy();
}

void VideoOutputShm::destroy()
{
  destroyXWindow();    

  destroyXImage();

}

void VideoOutputShm::fullscreen(bool fs)
{
  togglefullscreen(fs, _xRes, _yRes);
}

void VideoOutputShm::render(const Array2DType<RGBA> &image)
{    
  processX11Events();

  //only recreate xvimage if framesize change
  if (_frameSizeX!=image.width() || _frameSizeY!=image.height())
  {
    _frameSizeX=image.width();
    _frameSizeY=image.height();
    _frameSize=_frameSizeX*_frameSizeY;
    createXImage(_frameSizeX, _frameSizeY);
    resizeWindow(_frameSizeX, _frameSizeY);
  }


  unsigned char *data = (unsigned char*)image.data();

  if (_bpp == 24 || _bpp == 32)
  {
    unsigned char *xData = (unsigned char*)_xImage->data;
    for (int i=0;i<_frameSize;++i)
    {
      *xData++ = *(data+2);
      *xData++ = *(data+1);
      *xData++ = *(data);
      *xData++;
      data+=4;
    }
  } else//16bit  5-6-5 todo: handle with mask
  {
    unsigned short *xData = (unsigned short*)_xImage->data;
    unsigned short r,g,b;
    for (int i=0;i<_frameSize;++i)
    {
      b = *(data+2)>>3;
      g = *(data+1)>>2;
      r = (*data)>>3;
      *xData++ = (r << 11) | (g << 5) | b;
      data+=4;
    }
  }

  XShmPutImage((Display*)_display, _window, _gc, _xImage, 0, 0, 0, 0, _frameSizeX, _frameSizeY, False);

  XFlush((Display*)_display);
}

bool VideoOutputShm::init(int xRes, int yRes, bool fullscreen)
{            
  std::cout << "--==|| Shm output initialization ||==--" << std::endl;

  _xRes = xRes;
  _yRes = yRes;

  if (!openXDisplay())
    return false;

  if (!checkXShmExtension())
    return false;

  if (!createXWindow(xRes, yRes))
    return false;

  if (!createGC())
    return false;

  return true;
}

XImage* VideoOutputShm::createXImage(int sizeX, int sizeY)
{    
  destroyXImage();

  //we will create a ZPixmap format image -> 32bit
  int size = 4 * sizeX * sizeY;

  XShmSegmentInfo *shmInfo= createShmSegment(size);

  if (shmInfo==NULL)
    return NULL;

  if (shmInfo->shmaddr==NULL)
    return NULL;

  _xImage=XShmCreateImage((Display*)_display, _visualInfo.visual, _visualInfo.depth, ZPixmap, NULL, shmInfo, sizeX, sizeY);
  _xImage->data = shmInfo->shmaddr;    


  return _xImage;
}

void VideoOutputShm::destroyXImage()
{
  destroyShm();

  if (_xImage != NULL)
  {
    XFree(_xImage);
    _xImage = NULL;
  };
}

void VideoOutputShm::onResize(int sizeX, int sizeY)
{
  //force size to be of frame size
  resizeWindow(_frameSizeX, _frameSizeY);
}
