/* Gear_ColorQuantize.cpp
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#include "Gear_ColorQuantize.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ColorQuantize, Gear_ColorQuantize, "ColorQuantize")

const int Gear_ColorQuantize::MAX_COLOR = 256;

Gear_ColorQuantize::Gear_ColorQuantize(Engine *engine, std::string name)
  : Gear(engine, "ColorQuantize", name), _nColors(),
    Ir(0), Ig(0), Ib(0), Qadd(0)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "NColors", new ValueType(16, 2, 32)));
}

Gear_ColorQuantize::~Gear_ColorQuantize()
{
  free(Qadd);
  free(Ig);
  free(Ib);
  free(Ir);
}

bool Gear_ColorQuantize::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_ColorQuantize::runVideo()
{
  // initialize
  _nColors = CLAMP((int)_AMOUNT_IN->type()->value(), 2, MAX_COLOR);

  _image = _VIDEO_IN->type();
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  _data = _image->data();
  _outData = _outImage->data();

  size = _image->size();

  struct box  cube[MAX_COLOR];
  unsigned char *tag;
  unsigned char lut_r[MAX_COLOR], lut_g[MAX_COLOR], lut_b[MAX_COLOR];
  int   next;
  register long int i, weight;
  register int  k;
  float   vv[MAX_COLOR], temp;

  Qadd = (unsigned short int *)realloc(Qadd,sizeof(short int)*size);

  /* input R,G,B components into Ir, Ig, Ib;
     set size to width*height */

  Ir = (unsigned char*)realloc(Ir,size*sizeof(unsigned char));
  Ig = (unsigned char*)realloc(Ig,size*sizeof(unsigned char));
  Ib = (unsigned char*)realloc(Ib,size*sizeof(unsigned char));

  _imageIn = (unsigned char*)_data;
  for (i=0; i<size; ++i)
  {
    Ir[i] = *(_imageIn);
    Ig[i] = *(_imageIn+1);
    Ib[i] = *(_imageIn+2);
    _imageIn+=4;
  }

  memset(wt, 0, 33*33*33*sizeof(long int));
  memset(mr, 0, 33*33*33*sizeof(long int));
  memset(mg, 0, 33*33*33*sizeof(long int));
  memset(mb, 0, 33*33*33*sizeof(long int));
  memset(m2, 0, 33*33*33*sizeof(float));

  Hist3d((long int*)wt, (long int*)mr, (long int*)mg, (long int*)mb, (float*)m2); //printf("Histogram done\n");

  M3d((long int*)wt, (long int*)mr, (long int*)mg, (long int*)mb, (float*)m2);    //printf("Moments done\n");

  cube[0].r0 = cube[0].g0 = cube[0].b0 = 0;
  cube[0].r1 = cube[0].g1 = cube[0].b1 = 32;
  next = 0;
  for (i=1; i<_nColors; ++i)
  {
    if (Cut(&cube[next], &cube[i]))
    {
      /* volume test ensures we won't try to cut one-cell box */
      vv[next] = (cube[next].vol>1) ? Var(&cube[next]) : 0.0;
      vv[i] = (cube[i].vol>1) ? Var(&cube[i]) : 0.0;
    } else
    {
      vv[next] = 0.0;   /* don't try to split this box again */
      i--;              /* didn't create box i */
    }
    next = 0; temp = vv[0];
    for (k=1; k<=i; ++k)
      if (vv[k] > temp)
      {
        temp = vv[k]; next = k;
      }
    if (temp <= 0.0)
    {
      _nColors = i+1;
      fprintf(stderr, "Only got %d boxes\n", _nColors);
      break;
    }
  }
  //  printf("Partition done\n");

  /* the space for array m2 can be freed now */

  tag = (unsigned char *)malloc(33*33*33);
  if (tag==NULL)
  {
    printf("Not enough space\n"); exit(1);
  }
  for (k=0; k<_nColors; ++k)
  {
    Mark(&cube[k], k, tag);
    weight = Vol(&cube[k], wt);
    if (weight)
    {
      lut_r[k] = Vol(&cube[k], mr) / weight;
      lut_g[k] = Vol(&cube[k], mg) / weight;
      lut_b[k] = Vol(&cube[k], mb) / weight;
    } else
    {
      fprintf(stderr, "bogus box %d\n", k);
      lut_r[k] = lut_g[k] = lut_b[k] = 0;   
    }
  }

  _imageOut = (unsigned char*)_outData;
  for (i=0; i<size; ++i)
  {
    unsigned short int k = tag[Qadd[i]];
    *_imageOut++ = lut_r[k];
    *_imageOut++ = lut_g[k];
    *_imageOut++ = lut_b[k];
    *_imageOut++ = 0;
  }
  free(tag);

}
