/* VideoOutputXv.cpp
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

/* Inspired from the xvvideowindow class of jenskamenik@web.de Copyright (c) 2001, Jens Kamenik */

#include "VideoOutputXv.h"
#include "VideoOutputMaker.h"

#include <iostream>
using namespace X11;

Register_VideoOutput(MAKERVideoOutputXv, VideoOutputXv, "Xv")



VideoOutputXv::VideoOutputXv() :
VideoOutputX11Base(),
_xvImage(NULL),
_shmInfo(NULL),
_frameSizeX(0),
_frameSizeY(0),
_frameSize(0)
{
  memset(&_selectedFormat, 0, sizeof(XvImageFormatValuesPortPair));
}

VideoOutputXv::~VideoOutputXv()
{
  destroy();
}

void VideoOutputXv::destroy()
{
  destroyXWindow();    

  unGrabXvPort();

  destroyXvImage();

}

void VideoOutputXv::fullscreen(bool fs)
{
  togglefullscreen(fs, _xRes, _yRes);
}

void VideoOutputXv::render(const VideoRGBAType &image)
{    
  processX11Events();

  //only recreate xvimage if framesize change
  if (_frameSizeX!=(int)image.width() || _frameSizeY!=(int)image.height())
  {
    _frameSizeX=image.width();
    _frameSizeY=image.height();
    _frameSize=_frameSizeX*_frameSizeY;
    createXvImage(_frameSizeX, _frameSizeY);
    resizeWindow(_frameSizeX, _frameSizeY);
  }

  //todo rendering for other rgb formats..
  //this only work for 0x3 (RGB) 24.
  unsigned char *xvData = (unsigned char*)_xvImage->data;
  unsigned char *data = (unsigned char*)image.data();
  for (int i=0;i<_frameSize;++i)
  {
    *xvData++ = *(data+2);
    *xvData++ = *(data+1);
    *xvData++ = *(data);
    *xvData++;
    data+=4;
  }

  XvShmPutImage((Display*)_display, _selectedFormat.port, _window, _gc, _xvImage, 0, 0, _frameSizeX, _frameSizeY, 0, 0, _xRes, _yRes, False);
  
  waitForShmCompletion();
  
  XSync((Display*)_display, False);
}

bool VideoOutputXv::init(int xRes, int yRes, bool fullscreen)
{            
  std::cout << "--==|| Xv output initialization ||==--" << std::endl;

  _xRes = xRes;
  _yRes = yRes;

  if (!openXDisplay())
    return false;

  if (!checkXShmExtension())
    return false;

  if (!checkXvExtension())
    return false;

  if (!createXWindow(xRes, yRes))
    return false;

  if (!createGC())
    return false;

  if (!selectXvPort())
    return false;

  return true;
}

bool VideoOutputXv::checkXvExtension()
{    
  std::cout << "checking for Xv extension...";

  unsigned int ver=0, rev=0, eventB=0, reqB=0, errorB=0; 

  if (_display==NULL)
  {
    std::cout << "FAIL! no display" << std::endl;
    return false;
  }

  if (XvQueryExtension((Display*) _display, &ver, &rev, &reqB, &eventB, &errorB)!=Success)
  {
    std::cout << "FAIL! no Xv extension" << std::endl;
  }

  std::cout << "version " << ver << "." << rev << std::endl;
  return true;
}

bool VideoOutputXv::selectXvPort()
{
  std::cout << "- Xv select port -" << std::endl;

  if (_display==NULL)
  {
    std::cout << "FAIL! no display" << std::endl;
    return false;
  }

  if (_window==0)
  {
    std::cout << "FAIL! no window" << std::endl;
    return false;
  }

  unsigned int nbAdaptors=0;

  XvAdaptorInfo *adaptorInfo=NULL;

  std::cout << "selecting RGB format with highest depth..." << std::endl;
  //iterate on each format of each port of each adaptors to detect all rgbFormats
  XvQueryAdaptors((Display*)_display, _window, &nbAdaptors, &adaptorInfo);
  std::vector<XvImageFormatValuesPortPair> rgbFormats;
  for (unsigned int i=0; i<nbAdaptors; i++)
  {
    //does this port have the XvImageMask
    if (adaptorInfo[i].type & XvImageMask)
    {
      for (unsigned int j=0; j< adaptorInfo[i].num_ports; j++)
      {
        int port = adaptorInfo[i].base_id + j;


        int nbFormats=0;
        XvImageFormatValues* formatValues=XvListImageFormats((Display*) _display, port, &nbFormats);

        if (formatValues!=NULL)
        {
          for (int k = 0; k < nbFormats; k++)
          {
            //add a formatValues Port pair to the vector if of RGB type
            if (formatValues[k].type == XvRGB)
            {
              //make the pair
              XvImageFormatValuesPortPair rgbFormat;
              rgbFormat.format = formatValues[k];
              rgbFormat.port = port;

              rgbFormats.push_back(rgbFormat);
            }
          }                                       
          XFree(formatValues);    
        }
      }
    }
  }

  XvFreeAdaptorInfo(adaptorInfo);

  //return if no rgb format available
  if (rgbFormats.size()==0)
  {
    std::cout << "FAIL! no RGB formats detected" << std::endl;
    return false;
  }

  //now select the format with the highest depth
  memset(&_selectedFormat, 0, sizeof(XvImageFormatValuesPortPair));
  for (std::vector<XvImageFormatValuesPortPair>::iterator it=rgbFormats.begin(); it!=rgbFormats.end(); ++it)
  {
    if ((*it).format.depth > _selectedFormat.format.depth)
      _selectedFormat = (*it);
  }

  showFormat(_selectedFormat.format);

  std::cout << "grabbing port..";

  if (XvGrabPort((Display*) _display, _selectedFormat.port,0)!=Success)
  {
    std::cout << "FAIL!" << std::endl;
    return false;
  }


  std::cout << "done" << std::endl;


  return true;
}

void VideoOutputXv::showFormat(const XvImageFormatValues &format)
{
  std::cout << "XvFormat info:" << std::endl;
  std::cout << "ID : " << format.id << std::endl;
  std::cout << "Type : " << (format.type == XvRGB ? "RGB" : "YUV") << std::endl;
  std::cout << "Depth : " << format.depth << std::endl;
}

void VideoOutputXv::unGrabXvPort()
{
  std::cout << "unGrabbing Xv Port...";

  if (_selectedFormat.port==0)
  {
    std::cout << "not port grabbed" << std::endl;
    return;
  }

  XvUngrabPort((Display*)_display, _selectedFormat.port, 0);
  std::cout << "done" << std::endl;
}

XvImage* VideoOutputXv::createXvImage(int sizeX, int sizeY)
{    
  destroyXvImage();

  int size = (_selectedFormat.format.bits_per_pixel>>3) * sizeX * sizeY;
  
  _shmInfo = createShmSegment(size);
  
  if (_shmInfo==NULL)
    return NULL;

  if (_shmInfo->shmaddr==NULL)
    return NULL;

  _xvImage=XvShmCreateImage((Display*)_display, _selectedFormat.port, _selectedFormat.format.id,
                            (char *)_shmInfo->shmaddr, sizeX, sizeY, _shmInfo);
  
  XSync((Display*)_display, False);

  return _xvImage;
}

void VideoOutputXv::destroyXvImage()
{  
  if (_xvImage != NULL)
  {    
    destroyShm();
    XFree(_xvImage);
    _xvImage = NULL;
    XSync ((Display*)_display, False);
  };

}


