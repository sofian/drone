#ifndef CANVAS_INCLUDED
#define CANVAS_INCLUDED




typedef struct 
{
    unsigned char R,G,B,A;
}RGBA;

typedef struct 
{
    int R,G,B,A;
}RGBAint;

typedef struct 
{
    unsigned short R,G,B,A;
}RGBAshort;

typedef struct 
{
    float R,G,B,A;
}RGBAfloat;


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
