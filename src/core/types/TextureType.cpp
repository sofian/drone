#include "TextureType.h"

#if defined(Q_OS_MACX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#endif


TextureType::TextureType() :
  AbstractType(),
  _textureName(-1),
  _textureSizeX(0),
  _textureSizeY(0)
{
  
}

TextureType::~TextureType()
{
  if (_textureName != -1)
    glDeleteTextures(1, (GLuint*)&_textureName);
}


unsigned int TextureType::createWithSize(int textureSizeX, int textureSizeY, bool forceCreation)
{
  if (forceCreation || _textureSizeX != textureSizeX || _textureSizeY != textureSizeY)
  {
    if (_textureName != -1)
      glDeleteTextures(1, (GLuint*)&_textureName);
  }
  else if (_textureName != -1)
  {
    return _textureName;
  }
  
  _textureSizeX = textureSizeX;
  _textureSizeY = textureSizeY;
  std::cout << "Creating texture with size : " << _textureSizeX << "x" << _textureSizeY << std::endl;
  
  glGenTextures(1, (GLuint*)&_textureName);
  glBindTexture(GL_TEXTURE_2D, _textureName);
  
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _textureSizeX, _textureSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  
  return _textureName;
}

unsigned int TextureType::updateWithVideoRGBA(const VideoRGBAType &image)
{
  if (_textureName == -1 || _textureSizeX!=image.width() || _textureSizeY!=image.height())
  {
    std::cout << "image size changed, forcing texture creation" << std::endl;
    createWithSize(image.width(), image.height(), true);
  }

  glBindTexture(GL_TEXTURE_2D, _textureName);
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.data());
  
  return _textureName;  
}

