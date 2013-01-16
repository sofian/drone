/* Gear_Saturation.h
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

#ifndef GEAR_VecAffineTrans_INCLUDED
#define GEAR_VecAffineTrans_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"

#include <stdio.h>
#include <stdlib.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgba.h"
#include "platform/agg_platform_support.h"
#include "ctrl/agg_slider_ctrl.h"
#include "agg_svg_parser.h"
#include "VectorialType.h"

class Gear_VecAffineTrans : public Gear
{
public:

  Gear_VecAffineTrans();
  virtual ~Gear_VecAffineTrans();

  void runVideo();
  
private:


  agg::svg::path_renderer * m_path;
  
  PlugIn<VectorialType> *_VEC_IN; 
  PlugOut<VectorialType> *_VEC_OUT;
  PlugIn<ValueType> *_X_IN;
  PlugIn<ValueType> *_Y_IN;
  PlugIn<ValueType> *_SCALE_IN;
  PlugIn<ValueType> *_ROTATE_IN;

  double m_min_x;
  double m_min_y;
  double m_max_x;
  double m_max_y;
  
  double m_x;
  double m_y;
  
};

#endif
