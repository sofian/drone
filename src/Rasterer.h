#ifndef RASTERER_INCLUDED
#define RASTERER_INCLUDED

#include "MatrixType.h"
#include "ColorSpace.h"

class Rasterer
{
public:
  enum RasterMode
  {
    RASTER_REPLACE, RASTER_ADD, RASTER_SUB, RASTER_AND, RASTER_OR, RASTER_XOR
  };

  Rasterer();
  ~Rasterer();

  void setImage(MatrixType<RGBA> *image);    
  void setBrush(MatrixType<RGBA> *brush){_brush=brush;};
  void setColor(const RGBA &color){_color=color;};
  void setColor(unsigned char r, unsigned char g, unsigned char b);

  void setRasterMode(RasterMode rasterMode){_rasterMode=rasterMode;};

  void line(int x0, int y0, int x1, int y1);
  void rect(int x0, int y0, int x1, int y1, bool filled);
  void ellipse(int x0, int y0, int radx, int rady, bool filled);
  void fill(int x0, int y0);

  void ellipsePoints(int x0, int y0, int relx, int rely)
  {
    int x    = x0 + relx;
    int xSym = x0 - relx;
    int y    = y0 + rely;
    int ySym = y0 - rely;
    _image->get(y,x)       = _color;
    _image->get(y,xSym)    = _color;
    _image->get(ySym,x)    = _color;
    _image->get(ySym,xSym) = _color;
  }

private:

  MatrixType<RGBA> *_image;
  RGBA *_imageData;
  MatrixType<RGBA> *_brush; //!if brush = null then color is use to raster
  RGBA _color;

  RasterMode _rasterMode;


};

#endif
