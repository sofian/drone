#ifndef AGGWRAPPER_INCLUDED
#define AGGWRAPPER_INCLUDED

#include "VideoRGBAType.h"
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_outline_aa.h"
#include "agg_path_storage.h"
#include "agg_ellipse.h"
#include "agg_conv_transform.h"
#include "agg_bounding_rect.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"
#include "agg_pixfmt_rgba.h"

typedef agg::pixfmt_rgba32 pixfmt;
typedef agg::rgba8 color_type;
//typedef agg::order_rgba32 component_order;
typedef agg::renderer_base<pixfmt> renderer_base;
typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

class AggWrapper
{

public :
  AggWrapper(VideoRGBAType * outputImage);
  ~AggWrapper();

  void circle(float x, float y, float radius, agg::rgba8 color);
  void ellipse(float x, float y, float radiusx, float radiusy, agg::rgba8 color);
  void line(float x1, float y1, float x2, float y2, agg::rgba8 color);
  void quad(float x1, float y1, 
            float x2, float y2, 
            float x3, float y3,
            float x4, float y4, 
            agg::rgba8 color);

  
protected:
  pixfmt *_pixf;
  renderer_base *_rb;
  renderer_solid *_r;
  agg::rendering_buffer *_rbuf;

  VideoRGBAType *_outputImage; 
  
};

#endif
