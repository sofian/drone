//
//  DroneQGLWidget.h
//  drone
//
//  Created by Mathieu Guindon on 2012-12-23.
//  Copyright (c) 2012 drone. All rights reserved.
//

#ifndef __drone__DroneQGLWidget__
#define __drone__DroneQGLWidget__

#include <qgl.h>
#include <QtOpenGL/qglshaderprogram.h>
#include "ColorSpace.h"
#include "TextureType.h"

class Gear_VideoOutput;

class DroneQGLWidget : public QGLWidget
{
public:
  DroneQGLWidget(QWidget* parent, Gear_VideoOutput *videoOutput);
  ~DroneQGLWidget();
  
  void resize(int w, int h){resizeGL(w,h);}
  void setCurrentTexture(TextureType *currentTexture) {_currentTexture = currentTexture;}
  
protected:
  void	initializeGL();
  void	paintGL();
  void	resizeGL(int w, int h);
  
private:
  void initializeShaderProgram();
  
  TextureType *_currentTexture;
  unsigned int _frameSizeX;
  unsigned int _frameSizeY;
  unsigned int _frameSize;
  int _xRes;
  int _yRes;
  
  float _texSizeX, _texSizeY;
  QWidget *_parentWidget;
  
  bool _firstDraw;
	Gear_VideoOutput *_videoOutput;
  
  QGLShaderProgram _shaderProgram;
  int _vertexAttr;
  int _texCoordAttr;
  int _textureUniform;
};


#endif /* defined(__drone__DroneQGLWidget__) */
