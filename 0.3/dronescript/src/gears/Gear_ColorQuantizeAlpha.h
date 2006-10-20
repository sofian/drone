/* Gear_ColorQuantizeAlpha.h
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

#ifndef GEAR_COLORQUANTIZEALPHA_INCLUDED
#define GEAR_COLORQUANTIZEALPHA_INCLUDED

#include "Gear.h"
#include <assert.h>

#define DEFAULT_N_COLORS 16
#define CENTROIDS_THRESHOLD 10

// *** faire quelques tests et si ca marche toujours, enlever...
// *** parfois si on fait une seule iteration ca prend un certain temps... verifier la contrepartie en load average...
#define MAX_ITER 0
//#define DIST(x) (x >= 0 ? x : -x)

// *** a changer (fonction a la place ou direct dans le code)
#define DIST(x) (x*x)
//#define DIST(x) abs(x)

class PlugSignal;

class Gear_ColorQuantizeAlpha : public Gear  
{
public:
  Gear_ColorQuantizeAlpha(Schema *schema, std::string uniqueName);
  virtual ~Gear_ColorQuantizeAlpha();

  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};

private:

  PlugVideoIn *_VIDEO_IN;
  PlugVideoOut *_VIDEO_OUT;

  //local var
  Video_T *_image; 
  Video_T *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  int _nColors;
  RGBAint *_centroids;
#if MAX_ITER
  RGBAint *_prevCentroids;
#endif

  //selected centroids map for each pixels
  int *_clusters;

  //number of pixels falling in each centroids
  int *_clusterSizes;

  int _iterSizeY;
  int _iterSizeX;
  int _iterSize;

  unsigned char *_imageIn;
  unsigned char *_imageOut;    

};

#endif 
