#include "TextureType.h"

#if defined(Q_OS_MACX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#endif


TextureType::TextureType(unsigned int defaultValue) :
  AbstractType(),
  _textureName(defaultValue),
  _textureSizeX(0),
  _textureSizeY(0),
  _oldWidth(0),
  _oldHeight(0)
{
  
}

TextureType::~TextureType()
{
  if (_textureName)
  glDeleteTextures(1, (GLuint*)&_textureName);
}

unsigned int TextureType::createFromVideoRGBA(const VideoRGBAType &image, bool forceRecreate)
{
  if (!_textureName || forceRecreate ||
      _oldWidth!=image.width() || _oldHeight!=image.height())
  {
    std::cout << "create texture" << std::endl;
    
    _oldWidth=image.width();
    _oldHeight=image.height();
    //    _textureSizeX = ceilingPowerOfTwo(image.width());
    //    _textureSizeY = ceilingPowerOfTwo(image.height());
    _textureSizeX = image.width();
    _textureSizeY = image.height();
    
    std::cout << "Texture size : " << _textureSizeX << "x" << _textureSizeY << std::endl;
		
    if (_textureName)
      glDeleteTextures(1, (GLuint*)&_textureName);
    
    glGenTextures(1, (GLuint*)&_textureName);
    glBindTexture(GL_TEXTURE_2D, _textureName);
    
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    //32bit textures
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _textureSizeX, _textureSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }
  
  
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  
  
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.data());
  
  return _textureName;
  
}
