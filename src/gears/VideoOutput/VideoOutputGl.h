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

#include <qgl.h>
#include <qdialog.h>
#include "VideoOutput.h"
#include "TextureGl.h"

class DroneQGLWidget : public QGLWidget
{
public:
  DroneQGLWidget(QWidget* parent, VideoOutput *videoOutput);
  ~DroneQGLWidget();

  void resize(int w, int h){resizeGL(w,h);}
  void setCurrentImage(const VideoRGBAType &image){_currentImage = &image;}
  
protected:
  void	initializeGL();
  void	paintGL();
  void	resizeGL(int w, int h);

private:  
  const VideoRGBAType *_currentImage;
  unsigned int _frameSizeX;
  unsigned int _frameSizeY;
  unsigned int _frameSize;
  int _xRes;
  int _yRes;

  float _texSizeX, _texSizeY;
  QWidget *_parentWidget;
  TextureGl _textureGl;

  bool _firstDraw;
  RGBA _buffer[352*480];
	VideoOutput *_videoOutput;
};

//we need to create our own widget for the window containing the droneqglwidget
//to change the behavior of the close event to do nothing
//since we dont want this window to be closed by the user
class DroneGLWindow : public QDialog
{
public:  
  DroneGLWindow(QWidget* parent) : QDialog(parent)
  {
    setModal(false);
    resize(320,240);
  }
protected:
  void closeEvent(QCloseEvent *){}  
};

class VideoOutputGl : public VideoOutput
{
public:
  VideoOutputGl();
  ~VideoOutputGl();

  //videoOutput Overloading  
  bool init(int xRes, int yRes, bool fullscreen);
	bool toggleFullscreen(bool fs, int xRes, int yRes, int xPos, int yPos);

  void render(const VideoRGBAType &image);


protected:

  void onResize(int sizeX, int sizeY);
  
private:

  DroneQGLWidget *_droneQGLWidget;
  DroneGLWindow *_window;
  unsigned int _frameSizeX;
  unsigned int _frameSizeY;

  
};


#endif
