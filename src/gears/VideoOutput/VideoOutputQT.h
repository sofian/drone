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

#ifndef VIDEOOUTPUTOSX_INCLUDED
#define VIDEOOUTPUTOSX_INCLUDED

class Canvas;

#include "VideoOutput.h"

#include <qmainwindow.h>
#include <qimage.h>
#include <qapplication.h>

class RenderingWindow : public QMainWindow
{
public:
	RenderingWindow() : 
		QMainWindow(qApp->mainWidget(), 0, Qt::WType_Dialog | Qt::WStyle_StaysOnTop), 
		_frameSizeX(0), _frameSizeY(0), _frameSize(0)
	{
		_pixMap.setOptimization(QPixmap::BestOptim);
	}
	
	void updateVideoFrame(const VideoRGBAType &image);

protected:
	void paintEvent(QPaintEvent *paintEvent);
	
	QImage _videoFrame;
	QPixmap _pixMap;
	int _frameSizeX;
	int _frameSizeY;
	int _frameSize;

};

class VideoOutputQT : public VideoOutput
{
public:
  VideoOutputQT();
  ~VideoOutputQT();

  void fullscreen(bool fs);
  void destroy();
  bool init(int xRes, int yRes, bool fullscreen);
  void render(const VideoRGBAType &image);

protected:

//  void onResize(int sizeX, int sizeY);

private:
    
  RenderingWindow *_window;
};

#endif
