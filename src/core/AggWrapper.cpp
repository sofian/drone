#include "AggWrapper.h"
#include <stdlib.h>
#include <iostream>

template<class Source> struct stroke_fine
{
    typedef agg::conv_stroke<Source> stroke_type;

    stroke_type s;
  
    stroke_fine(Source& src, double w) : 
        s(src)
    {
        s.width(w); 
    }
    void rewind(unsigned id) { s.rewind(id); }
    unsigned vertex(double* x, double* y) { return s.vertex(x, y); }
};

struct line_t
{
    double x1, y1, x2, y2;
    int f;

    line_t(double x1_, double y1_, double x2_, double y2_) : 
        x1(x1_), y1(y1_), x2(x2_), y2(y2_), f(0) {}

    void rewind(unsigned) { f = 0; }
    unsigned vertex(double* x, double* y)
    {
        if(f == 0) { ++f; *x = x1; *y = y1; return agg::path_cmd_move_to; }
        if(f == 1) { ++f; *x = x2; *y = y2; return agg::path_cmd_line_to; }
        return agg::path_cmd_stop;
    }
};

class poly_t
{
public:
  double *xvec, *yvec;
  int num;
  int f;

  poly_t(double * x, double * y, int n):xvec(x),yvec(y)
    {
      num = n;
      // xvec = new double[num];
//       yvec = new double[num];
//       memcpy(xvec, x, num * sizeof(double));
//       memcpy(yvec, y, num * sizeof(double));
      
    }

  ~poly_t()
    {
      // delete []xvec;
//       delete []yvec;
    }

    void rewind(unsigned) { f = 0; }
    unsigned vertex(double* x, double* y)
    {
        if(f == 0) {  *x = xvec[0]; *y = yvec[0]; ++f;return agg::path_cmd_move_to; }
        if(f < num) {  *x = xvec[f]; *y = yvec[f]; ++f;return agg::path_cmd_line_to; }
        return agg::path_cmd_stop;
    }
};


AggWrapper::AggWrapper(VideoRGBAType * outputImage):_outputImage(outputImage)
{
  _rbuf = new agg::rendering_buffer(&((unsigned char*)_outputImage->data())[0], 
                               _outputImage->width(), 
                               _outputImage->height(), 
                               _outputImage->width() * 4);

  _pixf = new pixfmt(*_rbuf);
  _rb = new renderer_base(*_pixf);
  _r = new renderer_solid(*_rb);
}  

AggWrapper::~AggWrapper()
{
  delete _r;
  delete _rb;
  delete _pixf;
  delete _rbuf;
}

void AggWrapper::circle(float x, float y, float radius, agg::rgba8 color)
{
  ellipse( x,  y, radius, radius, color);
}

void AggWrapper::ellipse(float x, float y, float radiusx, float radiusy, agg::rgba8 color)
{
  agg::ellipse e1;
agg::conv_transform<agg::ellipse> some(e1,m_globtrans);
  agg::rasterizer_scanline_aa<> pf;
  agg::scanline_p8 sl;
  e1.init(x,y,radiusx,radiusy,(int)(radiusx+radiusy)/2+4);

  pf.add_path(some);
  _r->color(color);
  agg::render_scanlines(pf, sl, *_r);

}

void AggWrapper::line(float x1, float y1, float x2, float y2, agg::rgba8 color)
{
  agg::rasterizer_scanline_aa<> pf;
  agg::scanline_p8 sl;
  double x, y;

  line_t l(x1,y1,x2,y2);
  agg::conv_transform<line_t> some(l,m_globtrans);
  	
  stroke_fine<agg::conv_transform<line_t> > s(some, 1);
  some.rewind(0);
  while(!agg::is_stop(some.vertex(&x, &y)));
  
  pf.add_path(s);
  _r->color(agg::rgba8(color.r, color.g, color.b, color.a));
  agg::render_scanlines(pf, sl, *_r);



  
}


void AggWrapper::quad(float x1, float y1, 
                      float x2, float y2, 
                      float x3, float y3,
                      float x4, float y4, 
                      agg::rgba8 color)
{
  agg::rasterizer_scanline_aa<> pf;
  agg::scanline_p8 sl;
  double xvec[4]={x1,x2,x3,x4};
  double yvec[4]={y1,y2,y3,y4};
  double x,y;
  
  poly_t p((double*)&xvec,(double*)&yvec,4);
  //stroke_fine<poly_t> s(l, 1);
  p.rewind(0);
  while(!agg::is_stop(p.vertex(&x, &y)));
 
  pf.add_path(p);
  _r->color(agg::rgba8(color.r, color.g, color.b, color.a));
  agg::render_scanlines(pf, sl, *_r);
  
}
