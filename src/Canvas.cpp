// Canvas.cpp: implementation of the Canvas class.
//
//////////////////////////////////////////////////////////////////////

#include <string.h>

#include <GL/gl.h>              
#include <GL/glu.h>
#include <GL/glx.h>

#include "Canvas.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Canvas::Canvas() : 
    _data(NULL), 
    _sizeX(0), 
    _sizeY(0), 
    _texture(0),
    _textureSizeX(512),
    _textureSizeY(512)
{
}

Canvas::~Canvas()
{
    if (_data!=NULL)        
        delete[] _data;    

    if (_texture)
        glDeleteTextures(1, &_texture);
    
}


void Canvas::fill(unsigned char value)
{
    memset(_data, value, sizeof(RGBA) * _sizeX * _sizeY);
}

void Canvas::allocate(int sizex, int sizey)
{
    if (sizex==_sizeX && sizey==_sizeY)
        return;
    
    if (_data!=NULL)        
        delete[] _data;    

    _sizeX = sizex;
    _sizeY = sizey;
    _data = new RGBA[((int)sizex*(int)sizey)+1]; //+1 for mmx padding
}


void Canvas::fromRGB24(unsigned char *bufferRGB24)
{
    unsigned char *tempDataRGBA;
    unsigned char *tempDataRGB;
    for (int y=0;y<_sizeY;y++)
    {            
        tempDataRGBA = (unsigned char*)&_data[(y*_sizeX)];
        tempDataRGB = (bufferRGB24 + (y*_sizeX));

        for (int x=0;x<_sizeX;x++)
        {
            *(tempDataRGBA) = *(tempDataRGB);
            *(tempDataRGBA+1) = *(tempDataRGB+1);
            *(tempDataRGBA+2) = *(tempDataRGB+2);
            *(tempDataRGBA+3) = 0;

            tempDataRGBA+=4;
            tempDataRGB+=3;

        }   
    }
}

void Canvas::generateTexture(int bpp)
{
    glGenTextures(1, &_texture);		
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
          
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    
    if (bpp==16)        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5, _textureSizeX, _textureSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _textureSizeX, _textureSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}


GLuint Canvas::toTexture(int bpp)
{
    
    if (!_texture)
        generateTexture(bpp);
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _sizeX, _sizeY, GL_RGBA, GL_UNSIGNED_BYTE, _data);

    return _texture;
}
