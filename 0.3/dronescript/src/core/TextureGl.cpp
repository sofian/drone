  
#include "TextureGl.h"
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

using namespace GL;


TextureGl::TextureGl() :
  _texture(0),
  _textureSizeX(0),
  _textureSizeY(0),
  _oldWidth(0),
  _oldHeight(0)
{
}

TextureGl::~TextureGl()
{
  if (_texture)
    glDeleteTextures(1, (GLuint*)&_texture);
}

unsigned int TextureGl::createFromVideoRGBA(const VideoRGBAType &image, bool forceRecreate)
{
  if (!_texture || forceRecreate ||
      _oldWidth!=image.width() || _oldHeight!=image.height())
  {
    std::cout << "create texture" << std::endl;

    _oldWidth=image.width();
    _oldHeight=image.height();
    _textureSizeX = ceilingPowerOfTwo(image.width());
    _textureSizeY = ceilingPowerOfTwo(image.height());
    
    std::cout << "Texture size : " << _textureSizeX << "x" << _textureSizeY << std::endl;
		
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
      
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.data());

  return _texture;

}
