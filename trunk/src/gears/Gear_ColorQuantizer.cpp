/* Gear_ColorQuantizer.cpp
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

#include "Gear_ColorQuantizer.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ColorQuantizer, Gear_ColorQuantizer, "ColorQuantizer")

Gear_ColorQuantizer::Gear_ColorQuantizer(Schema *schema, std::string uniqueName) : Gear(schema, "ColorQuantizer", uniqueName),
_nColors(DEFAULT_N_COLORS),
_mapping(0), _initColors(false)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _centroids = new RGBAint[_nColors];//(RGBA*)malloc(_nColors*sizeof(RGBA));
  _prevCentroids = new RGBAint[_nColors];//(RGBA*)malloc(_nColors*sizeof(RGBA));
  std::cout << "N colors: " << _nColors << std::endl;

  // create initial centroids randomly
  RGBAint *iterCentroids = _centroids;
  for (int i=0;i<_nColors;++i)
  {
    iterCentroids->R = rand()%256;
    iterCentroids->G = rand()%256;
    iterCentroids->B = rand()%256;
    iterCentroids->A = 0;
    ++iterCentroids;

  }

  _nInMapping = (unsigned short*)malloc(_nColors*sizeof(unsigned short));
  _prevNInMapping = (unsigned short*)malloc(_nColors*sizeof(unsigned short));
  _logNInMapping = (float*)malloc(_nColors*sizeof(float));
}

Gear_ColorQuantizer::~Gear_ColorQuantizer()
{
  delete[] _centroids;
  delete[] _prevCentroids;
  free(_mapping);
  free(_nInMapping);
  free(_prevNInMapping);
  free(_logNInMapping);
}

bool Gear_ColorQuantizer::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_ColorQuantizer::runVideo()
{
  // init

  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->width(), _image->height());
  int size = _image->width() * _image->height();
  if (_mapping)
    _mapping = (unsigned short*) realloc(_mapping, size * sizeof(unsigned short));
  else
    _mapping = (unsigned short*) malloc(size * sizeof(unsigned short));

  _data = _image->_data;
  _outData = _outImage->_data;

  _iterSizeX = _image->width();
  _iterSizeY = _image->height();

  int centroidsDist;
  unsigned short *iterMapping;
  RGBAint *iterCentroids;

  float *iterLogNInMapping;
  int nUsedCentroids;
  for (int i=0; i<_nColors; ++i)
  {
    _prevNInMapping[i] = (_iterSizeX * _iterSizeY) / _nColors;
    _logNInMapping[i] = log(_prevNInMapping[i]);
  }

//   for (int i=0; i<_nColors; ++i)
//   {
//       int rnd = rand() % (_iterSizeX * _iterSizeY); // pick random point
//       _imageIn = (unsigned char*)&_data[rnd];
//       _centroids[i].R = (int)*(_imageIn);
//       _centroids[i].G = (int)*(_imageIn+1);
//       _centroids[i].B = (int)*(_imageIn+2);
//   }

  // k-means loop
  for (int i=0; i<MAX_ITER; ++i)
  {
    // cluster
    memset(_nInMapping, 0, _nColors*sizeof(unsigned short));
    iterMapping = _mapping;
    for (int y=0;y<_iterSizeY;y++)
    {
      _imageIn = (unsigned char*)&_data[y*_iterSizeX];
      for (int x=0;x<_iterSizeX;x++)
      {
        float minDist = 1e+16; // very big number
        iterCentroids = _centroids;
        iterLogNInMapping = _logNInMapping;
        float dist;
        for (int i=0; i<_nColors; ++i)
        {
          // *** might be optimized
          dist  = DIST( ((int)*(_imageIn)   - iterCentroids->R));
          dist += DIST( ((int)*(_imageIn+1) - iterCentroids->G));
          dist += DIST( ((int)*(_imageIn+2) - iterCentroids->B));
          dist *= _prevNInMapping[i];
          dist -= *iterLogNInMapping;
          if (dist < minDist)
          {
            *iterMapping = i;
            minDist = dist;
          }
          ++iterCentroids;
          ++iterLogNInMapping;
        }

        _imageIn+=4;

        _nInMapping[*iterMapping]++;
        ++iterMapping;
      }
    }

    // keep a copy of the centroids from previous iteration
    memcpy(_prevCentroids, _centroids, _nColors*sizeof(RGBAint));
    memcpy(_prevNInMapping, _nInMapping, _nColors*sizeof(unsigned short));

    // calculate new centroids
    nUsedCentroids = 0;
    for (int i=0; i<_nColors; ++i)
    {
      if (_nInMapping[i] > 0)
      {
        _centroids[i].R = 0;
        _centroids[i].G = 0;
        _centroids[i].B = 0;
        nUsedCentroids++;
      }
//       else // choose new color
//       {
//         int rnd = rand() % (_iterSizeX * _iterSizeY); // pick random point
//         _imageIn = (unsigned char*)&_data[rnd];
//         _centroids[i].R = (int)*(_imageIn);
//         _centroids[i].G = (int)*(_imageIn+1);
//         _centroids[i].B = (int)*(_imageIn+2);
//       }
    }

    //    std::cout << "Use " << nUsedCentroids << " colors on " << _nColors << std::endl;

    iterMapping = _mapping;
    for (int y=0;y<_iterSizeY;y++)
    {
      _imageIn = (unsigned char*)&_data[y*_iterSizeX];
      for (int x=0;x<_iterSizeX;x++)
      {
        iterCentroids = &_centroids[*iterMapping];
        if (_nInMapping[*iterMapping]) // ** ce if est pas necessaire
        {
          // *** pas efficace : il faut un autre encodage pour les centroides...
          iterCentroids->R += (int)*(_imageIn); // r
          iterCentroids->G += (int)*(_imageIn+1); // g
          iterCentroids->B += (int)*(_imageIn+2); // b
        }
        _imageIn+=4;
        ++iterMapping;
      }
    }

    iterCentroids = _centroids;
    RGBAint *iterPrevCentroids = _prevCentroids;
    unsigned short *iterNInMapping = _nInMapping;
    centroidsDist = 0;
    for (int i=0; i<_nColors; ++i)
    {
      if (*(iterNInMapping))
      {
        iterCentroids->R /= (*iterNInMapping);
        iterCentroids->G /= (*iterNInMapping);
        iterCentroids->B /= (*iterNInMapping);

        centroidsDist += DIST((iterPrevCentroids->R - iterCentroids->R));
        centroidsDist += DIST((iterPrevCentroids->G - iterCentroids->G));
        centroidsDist += DIST((iterPrevCentroids->B - iterCentroids->B));

      }
      ++iterPrevCentroids;
      ++iterCentroids;
      ++iterNInMapping;                                                              
    }

    centroidsDist /= _nColors;

    //    std::cout << centroidsDist << std::endl;

    if (centroidsDist < CENTROIDS_THRESHOLD)
      break;

    for (int i=0; i<_nColors; ++i)
      _logNInMapping[i] = (_nInMapping[i] == 0 ? 0. : log(_nInMapping[i]));
  }

//   for (int i=0; i<_nColors; ++i)
//   {
//     if (_nInMapping[i] == 0)
//     {
//       int rnd = rand() % (_iterSizeX * _iterSizeY); // pick random point
//       _imageIn = (unsigned char*)&_data[rnd];
//       _centroids[i].R = (int)*(_imageIn);
//       _centroids[i].G = (int)*(_imageIn+1);
//       _centroids[i].B = (int)*(_imageIn+2);
//     }
//   }

  //  std::cout << "remap" << std::endl;
  // remap colors
  iterMapping = _mapping;
  for (int y=0;y<_iterSizeY;y++)
  {
    _imageOut = (unsigned char*)&_outData[y*_iterSizeX];
    for (int x=0;x<_iterSizeX;x++)
    {
      iterCentroids  = &_centroids[*iterMapping];
      *(_imageOut)   = iterCentroids->R; // r
      *(_imageOut+1) = iterCentroids->G; // g
      *(_imageOut+2) = iterCentroids->B; // b
      *(_imageOut+3) = iterCentroids->A; // a

      _imageOut+=4;
      ++iterMapping;
    }
  }

}
