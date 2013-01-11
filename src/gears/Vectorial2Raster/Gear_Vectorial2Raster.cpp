/* Gear_Vectoria2Raster.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

enum { flip_y = true };

#include "Gear_Vectorial2Raster.h"
#include "Engine.h"

#include "Utils.h"

#include <iostream>
#include <fstream>

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, QString uniqueName)
{
  return new Gear_Vectorial2Raster(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "Vectorial2Raster";
  gearInfo.classification = GearClassifications::video().vectorial().instance();
  return gearInfo;
}
}

Gear_Vectorial2Raster::Gear_Vectorial2Raster(Schema *schema, QString uniqueName) : Gear(schema, "Vectorial2Raster", uniqueName)
{
  addPlug(_XOFF = new PlugIn<ValueType>(this, "xoff", false, new ValueType(0, -100, 100)));
  addPlug(_YOFF = new PlugIn<ValueType>(this, "yoff", false, new ValueType(0, -100, 100)));

  addPlug(_VEC_IN = new PlugIn<VectorialType>(this, "VecIN",true));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "BackG",true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT",true));
}

Gear_Vectorial2Raster::~Gear_Vectorial2Raster()
{

}

void Gear_Vectorial2Raster::runVideo()
{

  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  _imageIn  = (const unsigned char*) _image->data();
  _imageOut = (unsigned char*) _outImage->data();

  float xoff = _XOFF->type()->value();
  float yoff = _YOFF->type()->value();

  _vecIn  = _VEC_IN->type();
  
  zepath  = _vecIn->path();

  memcpy(&_imageOut[0],&_imageIn[0], _image->width() * _image->height() * 4);
  
  agg::rendering_buffer rbuf(&_imageOut[0], 
                               _image->width(), 
                               _image->height(), 
                               _image->width() * 4);

        typedef agg::pixfmt_rgba32 pixfmt;
        typedef agg::renderer_base<pixfmt> renderer_base;
        typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
 	
	

        pixfmt pixf(rbuf);
        renderer_base rb(pixf);
        renderer_solid ren(rb);

        //rb.clear(agg::rgba(1,1,1));

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;
        agg::trans_affine mtx;
	mtx *= agg::trans_affine_translation(xoff,yoff);
   
 
        zepath->render(ras, sl, ren, mtx, rb.clip_box(), 1.0);
/*
        char buf[128]; 
        agg::gsv_text t;
        t.size(10.0);
        t.flip(true);

        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.width(1.5);

        sprintf(buf, "Vertices=%d", m_path.vertex_count());

        t.start_point(10.0, 40.0);
        t.text(buf);

        ras.add_path(pt);
*/
        ren.color(agg::rgba(0,0,0));
        agg::render_scanlines(ras, sl, ren);

  


}

