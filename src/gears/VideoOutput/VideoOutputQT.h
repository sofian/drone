//Added by qt3to4:
#include <QPaintEvent>
#include <QPixmap>
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

class scene;

#include "VideoOutput.h"

#include <q3mainwindow.h>
#include <qimage.h>
#include <qapplication.h>

class RenderingWindow : public Q3MainWindow
{
public:
	RenderingWindow() : 
		Q3MainWindow(qApp->mainWidget(), 0, Qt::WType_Dialog | Qt::WStyle_StaysOnTop), 
		_frameSizeX(0), _frameSizeY(0), _frameSize(0)
	{
//		_pixMap.setOptimization(QPixmap::BestOptim);
	}
	
	void updateVideoFrame(const VideoRGBAType &image);

protected:
	void paintEvent(QPaintEvent *paintEvent);
	
	QImage _videoFrame;
	QPixmap _pixMap;
	unsigned int _frameSizeX;
	unsigned int _frameSizeY;
	unsigned int _frameSize;

};

class VideoOutputQT : public VideoOutput
{
public:
  VideoOutputQT();
  ~VideoOutputQT();
  
  bool init(int xRes, int yRes, bool fullscreen);
  void render(const VideoRGBAType &image);

protected:

//  void onResize(int sizeX, int sizeY);

private:
    
  RenderingWindow *_window;
};

#endif
