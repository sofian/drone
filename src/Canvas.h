#ifndef CANVAS_INCLUDED
#define CANVAS_INCLUDED




struct RGBA
{
    unsigned char R,G,B,A;
};

struct RGBAint
{
    int R,G,B,A;
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
    
    int sizeX(){return _sizeX;};
    int sizeY(){return _sizeY;};
    
    void fill(unsigned char value=0);
    void fromRGB24(unsigned char *bufferRGB24);

    RGBA* pix(int x, int y)
    {
        return &_data[(y*_sizeX) + x];
    }

    void allocate(int sizex, int sizey);
    
    unsigned int toTexture();
    
private:
    
    void generateTexture();

    int _sizeX, _sizeY;
    
    
    unsigned int _texture;
};

#endif 
