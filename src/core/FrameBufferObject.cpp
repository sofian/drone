//
//  FrameBufferObject.cpp
//  drone
//
//  Created by Mathieu Guindon on 2012-12-26.
//  Copyright (c) 2012 drone. All rights reserved.
//

#include "FrameBufferObject.h"

#include <qgl.h>

#if defined(Q_OS_MACX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#endif

#include <iostream>

FrameBufferObject::FrameBufferObject() :
  _frameBuffer(-1)
{
  
}

FrameBufferObject::~FrameBufferObject()
{
  destroy();
}

void FrameBufferObject::create()
{
  if (_frameBuffer != -1)
  {
      std::cerr << "FrameBufferObject already created." << std::endl;
      return;
  }
  
  glGenFramebuffers(1, (GLuint*)&_frameBuffer);
}

void FrameBufferObject::destroy()
{
  if (_frameBuffer == -1)
    return;
  
  unBind();
  glDeleteFramebuffers(1, (GLuint*)&_frameBuffer);
}

bool FrameBufferObject::isReady()
{
  return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void FrameBufferObject::attachTexture(unsigned int textureName)
{
  if (_frameBuffer == -1)
  {
    std::cerr << "Fail to attach texture to frameBuffer, call create first." << std::endl;
    return;
  }
 
  bind();

  glBindTexture(GL_TEXTURE_2D, textureName);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureName, 0);
  
  unBind();
}

void FrameBufferObject::bind()
{
  if (_frameBuffer == -1)
  {
    std::cerr << "Fail to attach texture to frameBuffer, call create first." << std::endl;
    return;
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
}

void FrameBufferObject::unBind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}