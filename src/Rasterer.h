#ifndef RASTERER_INCLUDED
#define RASTERER_INCLUDED

#include "Canvas.h"

class Rasterer
{
public:
    enum RasterMode {RASTER_REPLACE, RASTER_ADD, RASTER_SUB, RASTER_AND, RASTER_OR, RASTER_XOR};
        
    Rasterer();
    ~Rasterer();

    void setCanvas(Canvas *canvas);    
    void setBrush(Canvas *brush){_brush=brush;};
    void setColor(const RGBA &color){_color=color;};
    void setColor(unsigned char r, unsigned char g, unsigned char b);
        
    void setRasterMode(RasterMode rasterMode){_rasterMode=rasterMode;};
    
    void line(int x0, int y0, int x1, int y1);
    void rect(int x0, int y0, int x1, int y1, bool filled);
    void circle(int x0, int y0, int radius, bool filled);
    void fill(int x0, int y0);

private:

    Canvas *_canvas;
    RGBA *_canvasData;
    Canvas *_brush; //!if brush = null then color is use to raster
    RGBA _color;

    RasterMode _rasterMode;
    
    
};

#endif
