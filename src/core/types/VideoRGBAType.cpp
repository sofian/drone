#include "VideoRGBAType.h"
namespace GL
{
#if defined(Q_OS_MACX)
#include <OpenGL/gl.h>              
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>              
#endif
}

#include <iostream>
#include "Utils.h"
#include "AlphaComposite.h"

using namespace GL;

VideoRGBAType::~VideoRGBAType() 
{
  if (_texture)  
    glDeleteTextures(1, (GLuint*)&_texture);
}

void VideoRGBAType::toGray()
{
  if (!isGray())
  {
    grayscaleRGBA(data(), data(), size());
    setIsGray(true);
  }
}

void VideoRGBAType::premultiplyAlpha()
{
  if (!isAlphaPremultiplied())
  {
    alpha_premultiply((unsigned char*)data(), size());
    setIsAlphaPremultiplied(true);
  }
}

void VideoRGBAType::demultiplyAlpha()
{
  if (isAlphaPremultiplied())
  {
    alpha_demultiply((unsigned char*)data(), size());
    setIsAlphaPremultiplied(false);
  }
}

void VideoRGBAType::setAlpha(const Array<unsigned char>& mask)
{
  ASSERT_ERROR (size() != mask.size());
  if (isAlphaPremultiplied())
  {
    demultiplyAlpha();
    alpha_set((unsigned char*)data(), mask.data(), size());
    premultiplyAlpha();
  }
  else
    alpha_set((unsigned char*)data(), mask.data(), size());
}

void VideoRGBAType::fillAlpha(unsigned char alpha)
{
  if (isAlphaPremultiplied())
  {
    demultiplyAlpha();
    alpha_fill((unsigned char*)data(), alpha, size());
    premultiplyAlpha();
  }
  else
    alpha_fill((unsigned char*)data(), alpha, size());
}

unsigned int VideoRGBAType::toTexture(bool forceRecreate) const
{
  if (!_texture || forceRecreate)
  {
    std::cout << "create texture" << std::endl;
    if (_texture)
      glDeleteTextures(1, (GLuint*)&_texture);
    
    glGenTextures(1, (GLuint*)&_texture);		
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
          
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //32bit textures
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _textureSizeX, _textureSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);    
  }
      
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, data());

  return _texture;
}

