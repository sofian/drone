#ifndef CANVAS_INCLUDED
#define CANVAS_INCLUDED

struct RGBA
{
    unsigned char R,G,B,A;
};

struct RGBAint
{
  int R,G,B,A;
  
  void operator+=(const RGBAint& x)
  {
    int *ptr1 = (int*)this, *ptr2 = (int*)&x;
    *ptr1++ += *ptr2++; *ptr1++ += *ptr2++;
    *ptr1++ += *ptr2++; *ptr1   += *ptr2;
  }

  void operator-=(const RGBAint& x)
  {
    int *ptr1 = (int*)this, *ptr2 = (int*)&x;
    *ptr1++ -= *ptr2++; *ptr1++ -= *ptr2++;
    *ptr1++ -= *ptr2++; *ptr1   -= *ptr2;
  }

  void operator*=(const RGBAint& x)
  {
    int *ptr1 = (int*)this, *ptr2 = (int*)&x;
    *ptr1++ *= *ptr2++; *ptr1++ *= *ptr2++;
    *ptr1++ *= *ptr2++; *ptr1   *= *ptr2;
  }

};

struct RGBAshort
{
    unsigned short R,G,B,A;
};

struct RGBAfloat
{
    float R,G,B,A;
};

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
