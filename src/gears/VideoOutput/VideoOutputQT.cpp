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

#include "VideoOutputQT.h"

#include <iostream>

#include <qpainter.h>
#include <qthread.h>
//Added by qt3to4:
#include <QPaintEvent>

//maker strategy disabled on osx
#ifndef Q_OS_MACX
#include "VideoOutputMaker.h"
Register_VideoOutput(MAKERVideoOutputQT, VideoOutputQT, "QT")
#endif

void RenderingWindow::updateVideoFrame(const VideoRGBAType &image)
{
	//first build a qimage
	if (image.width() != _frameSizeX || image.height() != _frameSizeY)
	{	
		_frameSizeX = image.width();
		_frameSizeY = image.height();
		_frameSize = _frameSizeX * _frameSizeY;
		_videoFrame.create(_frameSizeX, _frameSizeY, 32);
		//resize the window
		resize(_frameSizeX, _frameSizeY);
	}
	
	//fill the qimage with data from image
	unsigned char *bits = _videoFrame.bits();
	RGBA *data = image.data();
	
	for (unsigned int i=0;i<_frameSize;++i)
	{
		(*(QRgb*)bits) = qRgb(data->r, data->g, data->b); 
		bits+=4;
		data++;
	}
	
	//convert the qimage to a pixmap that will be rendered more efficiently
	_pixMap.convertFromImage(_videoFrame, 0);
	
	//asynchronously tell the widget to repaint itself in a thread-safe way 
	QCoreApplication::postEvent(this, new QPaintEvent( QRect(0, 0, _frameSizeX, _frameSizeY) ) );
}

void RenderingWindow::paintEvent(QPaintEvent *)
{
	//simply use the painter to blit the pixmap to the screen
	QPainter painter(this);
	painter.drawPixmap(0,0, _pixMap);
}


VideoOutputQT::VideoOutputQT() :
	VideoOutput()
{    
}

VideoOutputQT::~VideoOutputQT()
{
  delete _window;
}

void VideoOutputQT::render(const VideoRGBAType &image)
{    	
	//tell the rendering window that we have a new frame
	_window->updateVideoFrame(image);
}

bool VideoOutputQT::init(int xRes, int yRes, bool)
{
	_window = new RenderingWindow();
	_window->resize(xRes, yRes);
	_window->show();
	
	return true;
}



//unused osx native stuff
/*
bool VideoOutputOSX::init(int xRes, int yRes, bool fullscreen)
{            
  std::cout << "--==|| OSX output initialization ||==--" << std::endl;

  _xRes = xRes;
  _yRes = yRes;
  
  Rect windowBounds = { 40, 20, _yRes, _xRes};
  Rect movieRect = {0, 0, 768, 1024};

  //GetGWorld(&_savedPort, &_savedGDevice);	

	
	 if (NewGWorld(&_offscreen, k32ARGBPixelFormat, &movieRect, NULL, NULL, 0) != noErr)
	 {
		 std::cout << "fail to create gworld for video output!" << std::endl;
		 return false;
	 }
	 _pixMap = GetGWorldPixMap(_offscreen);
	 _baseAddr = (unsigned char *) GetPixBaseAddr(_pixMap);	
	 _rowStride = (unsigned long) GetPixRowBytes (_pixMap);
	 
  //SetGWorld(_savedPort, _savedGDevice);
  
	CreateNewWindow (kDocumentWindowClass, kWindowStandardHandlerAttribute , &windowBounds, &_window);	
	SetWindowTitleWithCFString(_window,CFSTR("video output"));
	ShowWindow(_window);
	
	SetPort(GetWindowPort(_window));
  
	_visibleRgn=NewRgn();
	return true;
}
*/
/*
 void VideoOutputOSX::render(const VideoRGBAType &image)
 {    
	 
	 
	 if (_frameSizeX!=(int)image.width() || _frameSizeY!=(int)image.height())
	 {
		 
		 _frameSizeX=image.width();
		 _frameSizeY=image.height();
		 _frameSize=_frameSizeX*_frameSizeY;
		 
		 _movieRect.top=0;_movieRect.bottom=_frameSizeY;
		 _movieRect.left=0;_movieRect.right=_frameSizeX;
	 }		
	 
	 unsigned char *tempimg = (unsigned char*)image.data();
	 unsigned char *row; 
	 for(int y=0; y<_frameSizeY; y++)
	 {
		 row = _baseAddr + y*_rowStride;
		 for(int x=0; x<_frameSizeX; x++)
		 {
			 row++;
			 *(row++) = *(tempimg++);
			 *(row++) = *(tempimg++);
			 *(row++) = *(tempimg++);
			 tempimg++;
		 }
	 }
	 
	 
	 GrafPtr origPort;
	 Rect windowBounds;
	 GetPort(&origPort);
	 SetPortWindowPort(_window);
	 
	 if(!LockPixels(GetPortPixMap(GetWindowPort(_window))))
	 {
		 std::cout << "fail to lock pixmap" << std::endl;
		 return;
	 }
	 
	 GetPortBounds(GetWindowPort(_window), &windowBounds);
	 CopyBits((BitMap*) *_pixMap, GetPortBitMapForCopyBits(GetWindowPort(_window)), &_movieRect, &windowBounds, srcCopy, NULL);
	 
	 UnlockPixels(GetPortPixMap(GetWindowPort(_window)));
	 
	 QDFlushPortBuffer(GetWindowPort(_window), GetPortVisibleRegion(GetWindowPort(_window), _visibleRgn));
	 SetPort(origPort);	
	 
 }
 */
