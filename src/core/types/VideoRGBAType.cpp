#include "VideoRGBAType.h"
namespace GL
{
#if defined(Q_OS_MACX)
#include <OpenGL/gl.h>              
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>              
  //#include <GL/glu.h>
#endif
}

using namespace GL;

unsigned int VideoRGBAType::toTexture() const
{
  if (!_texture)
  {
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

