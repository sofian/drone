#ifndef CANVAS_INCLUDED
#define CANVAS_INCLUDED

#include "ColorSpace.h"

class Canvas  
{
public:
    static const int MAX_SIZEX;
    static const int MAX_SIZEY;

    Canvas();
	~Canvas();
    
    RGBA* _data;
    
    int sizeX() const {return _sizeX;};
    int sizeY() const {return _sizeY;};

    int textureSizeX() const {return _textureSizeX;}
    int textureSizeY() const {return _textureSizeY;}
        
    void fill(unsigned char value=0);
    void fromRGB24(unsigned char *bufferRGB24);

    RGBA* pix(int x, int y) const
    {
        return &_data[(y*_sizeX) + x];
    }

    void allocate(int sizex, int sizey);
    
    unsigned int toTexture(int bpp=32);
    
private:
    
    void generateTexture(int bpp);

    int _sizeX, _sizeY;
    
    
    unsigned int _texture;
    int _textureSizeX;
    int _textureSizeY;
};

#endif 
