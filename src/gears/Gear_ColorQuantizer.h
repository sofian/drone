/* Gear_ColorQuantizer.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
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

#ifndef GEAR_COLORQUANTIZER_INCLUDED
#define GEAR_COLORQUANTIZER_INCLUDED

#include "Gear.h"
#include <assert.h>

#define DEFAULT_N_COLORS 16
#define CENTROIDS_THRESHOLD 10
#define MAX_ITER 10
//#define DIST(x) (x >= 0 ? x : -x)
#define DIST(x) (x*x)


class PlugSignal;

class Gear_ColorQuantizer : public Gear  
{
public:
  Gear_ColorQuantizer(Schema *schema, std::string uniqueName);
  virtual ~Gear_ColorQuantizer();

  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};

  bool ready();

private:

  PlugVideoIn *_VIDEO_IN;
  PlugVideoOut *_VIDEO_OUT;

  //local var
  Video_T *_image; 
  Video_T *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  unsigned short _nColors;
  RGBAint *_centroids;
  RGBAint *_prevCentroids;

  //selected centroids map for each pixels
  unsigned short *_mapping;

  //number of pixels falling in each centroids
  unsigned short *_nInMapping;
  unsigned short *_prevNInMapping;

  float *_logNInMapping;

  bool _initColors;

  int _iterSizeY;
  int _iterSizeX;

  int *_mmxImageIn;
  int *_mmxImageOut;
  unsigned long long int _mmxColor;

  short _r;
  short _g;
  short _b;
  short _a;

  unsigned char *_imageIn;
  unsigned char *_imageOut;    

};

#endif 
