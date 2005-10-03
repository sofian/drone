/* VideoOutputGl.cpp
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

#include "VideoOutputGl.h"
#include "VideoOutputMaker.h"

#include <iostream>

#if defined(Q_OS_MACX)
#include <OpenGL/gl.h>              
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>              
#endif

#include <qapplication.h>
#include <qlayout.h>

DroneQGLWidget::DroneQGLWidget(QWidget* parent) : QGLWidget(parent, "video out"),
_currentImage(NULL),
_frameSizeX(0),
_frameSizeY(0),
_frameSize(0),
_xRes(0),
_yRes(0),
_parentWidget(parent),
_firstDraw(true)
{
}

DroneQGLWidget::~DroneQGLWidget()
{
  doneCurrent();
}

void DroneQGLWidget::initializeGL()
{
  glEnable(GL_TEXTURE_2D);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST); 
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);         
  glDisable(GL_DITHER);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
}

void DroneQGLWidget::paintGL()
{
  
  if (_currentImage==NULL)
  {
      glClear(GL_COLOR_BUFFER_BIT);//black background if no image
      return;
  }
    
  
  if (_frameSizeX!=_currentImage->width() || _frameSizeY!=_currentImage->height())  
  {
    _frameSizeX = _currentImage->width();
    _frameSizeY = _currentImage->height();
    
    //tell the parent to resize, it will also resize us, calling resizeGL since we will receive the event
    //we do this here, because we cannot make this call from an external thread
    _parentWidget->resize(_frameSizeX, _frameSizeY);    
  }

  _texSizeX = (float)_frameSizeX / (float)_textureGl.textureSizeX();
  _texSizeY = (float)_frameSizeY / (float)_textureGl.textureSizeY();

  glEnable(GL_TEXTURE_2D);
  
  //if this is the first time we draw, we have to force texture recreation since we are in
  //a newly created gl context
  if (_firstDraw)
  { 
    glBindTexture(GL_TEXTURE_2D, _textureGl.createFromVideoRGBA(*_currentImage, true));
    _firstDraw=false;   
  }   
  else
    glBindTexture(GL_TEXTURE_2D, _textureGl.createFromVideoRGBA(*_currentImage));
  
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0.0f, 0.0f);

  glTexCoord2f(_texSizeX, 0.0f);
  glVertex2f(_xRes, 0.0f);

  glTexCoord2f(_texSizeX, _texSizeY);
  glVertex2f(_xRes, _yRes);

  glTexCoord2f(0.0f, _texSizeY);
  glVertex2f(0.0f, _yRes);

  glEnd();

}

void DroneQGLWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, h, 0, -99999, 99999);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  _xRes=w;
  _yRes=h;
}


Register_VideoOutput(MAKERVideoOutputGL, VideoOutputGl, "Gl")

VideoOutputGl::VideoOutputGl() :
VideoOutput(),    
_droneQGLWidget(NULL),
_window(NULL),
_frameSizeX(0),
_frameSizeY(0)
{    
}

VideoOutputGl::~VideoOutputGl()
{  
  //we are not in the gui thread, so we must schedule the delete instead of processing it now
  _window->deleteLater();
}

void VideoOutputGl::render(const VideoRGBAType &image)
{        
  if (_frameSizeX!=image.width() || _frameSizeY!=image.height())
  {
    _frameSizeX = image.width();
    _frameSizeY = image.height();           
  }
  
  _droneQGLWidget->setCurrentImage(image);
  
  //asynchronously tell the widget to repaint himself in a thread-safe way 
  QApplication::postEvent(_droneQGLWidget, new QPaintEvent( QRect(0, 0, _frameSizeX, _frameSizeY) ) );  
}

bool VideoOutputGl::init(int xRes, int yRes, bool fullscreen)
{            
  std::cout << "--==|| GL output initialization ||==--" << std::endl;
  
  _xRes = xRes;
  _yRes = yRes;

  if (_window)
    delete _window;
    
  _window = new DroneGLWindow(qApp->mainWidget()); 
  _droneQGLWidget = new DroneQGLWidget(_window);  
  QBoxLayout *l = new QHBoxLayout(_window);
  l->addWidget(_droneQGLWidget);  
  _window->setModal(false);
  _window->show();
  

  return true;
}

void VideoOutputGl::onResize(int sizeX, int sizeY)
{    
}

