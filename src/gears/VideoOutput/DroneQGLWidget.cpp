//
//  DroneQGLWidget.cpp
//  drone
//
//  Created by Mathieu Guindon on 2012-12-23.
//  Copyright (c) 2012 drone. All rights reserved.
//

#include "DroneQGLWidget.h"
#include "Gear_VideoOutput.h"

#if defined(Q_OS_MACX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#endif

#include <qapplication.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3BoxLayout>
#include <Q3HBoxLayout>
#include <QPaintEvent>

GLfloat afVertices[] = {
  -1,-1,0,
  -1,1,0,
  1,-1,0,
  1,1,0,
};

//tex coord are Y-flipped
GLfloat afTexCoord[] = {
  0,1,
  0,0,
  1,1,
  1,0
};


DroneQGLWidget::DroneQGLWidget(QWidget* parent, Gear_VideoOutput* videoOutput) : QGLWidget(parent, "video out"),
_currentTexture(NULL),
_frameSizeX(0),
_frameSizeY(0),
_frameSize(0),
_xRes(0),
_yRes(0),
_parentWidget(parent),
_firstDraw(true),
_videoOutput(videoOutput)
{
}

DroneQGLWidget::~DroneQGLWidget()
{
  doneCurrent();
}

void DroneQGLWidget::initializeShaderProgram()
{
  const char *vsrc1 =
  "attribute vec3 vertex;\n"
  "attribute vec2 texCoord;\n"
  "varying vec2 texc;\n"
  "void main(void)\n"
  "{\n"
  "    texc = texCoord;\n"
  "    gl_Position = vec4(vertex.x, vertex.y, vertex.z, 1.0);\n"
  "}\n";
  
  const char *fsrc1 =
  "varying vec2 texc;\n"
  "uniform sampler2D tex;\n"
  "void main(void)\n"
  "{\n"
  "    gl_FragColor = texture2D(tex, texc);\n"
  "}\n";
  
  _shaderProgram.addShaderFromSourceCode(QGLShader::Vertex, vsrc1);
  _shaderProgram.addShaderFromSourceCode(QGLShader::Fragment, fsrc1);
  _shaderProgram.link();
  
  _vertexAttr = _shaderProgram.attributeLocation("vertex");
  _texCoordAttr = _shaderProgram.attributeLocation("texCoord");
  _textureUniform = _shaderProgram.uniformLocation("tex");
  
}

void DroneQGLWidget::initializeGL()
{
  initializeShaderProgram();
  
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DITHER);
  glDisable(GL_DEPTH_TEST);
}

void DroneQGLWidget::paintGL()
{
  if (_currentTexture == NULL)
  {
    glClear(GL_COLOR_BUFFER_BIT);//black background if no image
    return;
  }
  
  if (_frameSizeX!=_currentTexture->textureSizeX() || _frameSizeY!=_currentTexture->textureSizeY())
  {
    _frameSizeX = _currentTexture->textureSizeX();
    _frameSizeY = _currentTexture->textureSizeY();
    
    //tell the parent to resize, it will also resize us, calling resizeGL since we will receive the event
    //we do this here, because we cannot make this call from an external thread
    if (!_videoOutput->isFullscreen())
			_parentWidget->resize(_frameSizeX, _frameSizeY);
  }
  
  glViewport(0, 0, _xRes, _yRes);
  
  _shaderProgram.bind();
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  
  glBindTexture(GL_TEXTURE_2D, _currentTexture->textureName());
  
  _shaderProgram.setUniformValue(_textureUniform, 0);
  _shaderProgram.enableAttributeArray(_vertexAttr);
  _shaderProgram.enableAttributeArray(_texCoordAttr);
  _shaderProgram.setAttributeArray(_vertexAttr, afVertices, 3);
  _shaderProgram.setAttributeArray(_texCoordAttr, afTexCoord, 2);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  //todo:foogy
  //swapBuffer, disable autoswap?
}

void DroneQGLWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  _xRes=w;
  _yRes=h;
}
