/* Gear_ColorQuantizeAlpha.cpp
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#include "Gear_ColorQuantizeAlpha.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ColorQuantizeAlpha, Gear_ColorQuantizeAlpha, "ColorQuantizeAlpha")

Gear_ColorQuantizeAlpha::Gear_ColorQuantizeAlpha(Engine *engine, std::string name)
: Gear(engine, "ColorQuantizeAlpha", name), _nColors(DEFAULT_N_COLORS), _clusters(0)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");

  _centroids = new RGBAint[_nColors];
#if MAX_ITER
  _prevCentroids = new RGBAint[_nColors];
#endif

  srand(2732791);


  // create initial centroids randomly
  for (int i=0;i<_nColors;++i)
  {
    _centroids[i].R = rand()%256;
    _centroids[i].G = rand()%256;
    _centroids[i].B = rand()%256;
    _centroids[i].A = 0;
  }

  _clusterSizes = (int*)malloc(_nColors*sizeof(int));
}

Gear_ColorQuantizeAlpha::~Gear_ColorQuantizeAlpha()
{
  delete[] _centroids;
#if MAX_ITER
  delete[] _prevCentroids;
#endif
  free(_clusters);
  free(_clusterSizes);
}

bool Gear_ColorQuantizeAlpha::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_ColorQuantizeAlpha::runVideo()
{
  // init

  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->width(), _image->height());

  _iterSizeX = _image->width();
  _iterSizeY = _image->height();
  _iterSize = _iterSizeX * _iterSizeY;

  if (_clusters)
    _clusters = (int*) realloc(_clusters, _iterSize * sizeof(int));
  else
    _clusters = (int*) malloc(_iterSize * sizeof(int));

  _data = _image->_data;
  _outData = _outImage->_data;

#if MAX_ITER
  int centroidsDist;
#endif
  int *iterClusters;
  int *iterCentroids;
  int *iterClusterSizes;
#if MAX_ITER
  int *iterPrevCentroids;
#endif
  int nUsedCentroids;

  // k-means loop
#if MAX_ITER
  for (int i=0; i<MAX_ITER; ++i)
#endif
  {
    // cluster
    memset(_clusterSizes, 0, _nColors*sizeof(int));
    iterClusters = _clusters;
    for (int y=0;y<_iterSizeY;y++)
    {
      _imageIn = (unsigned char*)&_data[y*_iterSizeX];
//       int _r = (int)_imageIn[0];
//       int _g = (int)_imageIn[1];
//       int _b = (int)_imageIn[2];
//       std::cout << "alpha[" << (y*_iterSizeX) << "] = " << _r << "," << _g << "," << _b << std::endl;
      for (int x=0;x<_iterSizeX;x++)
      {
        int minDist = INT_MAX; // very big number
        iterCentroids = (int*)_centroids;
        int dist;
        for (int i=0; i<_nColors; ++i)
        {
          dist  = DIST( ((int)*(_imageIn)   - *(iterCentroids)) );
          dist += DIST( ((int)*(_imageIn+1) - *(iterCentroids+1)) );
          dist += DIST( ((int)*(_imageIn+2) - *(iterCentroids+2)) );
          if (dist < minDist)
          {
            *iterClusters = i;
            minDist = dist;
          }
          iterCentroids+=4;
        }
        //        std::cout << "mindist = " << minDist << std::endl;
        _imageIn+=4;

        _clusterSizes[*iterClusters]++;
        ++iterClusters;
      }
    }

#if MAX_ITER
    // keep a copy of the centroids from previous iteration
    memcpy(_prevCentroids, _centroids, _nColors*sizeof(RGBAint));
#endif

    // calculate new centroids
    nUsedCentroids = 0;
    iterCentroids = (int*)_centroids;
    iterClusterSizes = _clusterSizes;
    for (int i=0; i<_nColors; ++i)
    {
      if (*iterClusterSizes > 0)
      {
        memset(iterCentroids, 0, sizeof(RGBAint));
        nUsedCentroids++;
      }
      ++iterClusterSizes;
    }

    iterClusters = _clusters;
    for (int y=0;y<_iterSizeY;y++)
    {
      _imageIn = (unsigned char*)&_data[y*_iterSizeX];
      for (int x=0;x<_iterSizeX;x++)
      {
        iterCentroids = (int*)&_centroids[*iterClusters];
        *(iterCentroids)   += (int)*(_imageIn); // r
        *(iterCentroids+1) += (int)*(_imageIn+1); // g
        *(iterCentroids+2) += (int)*(_imageIn+2); // b

        _imageIn+=4;
        ++iterClusters;
      }
    }

    iterCentroids = (int*)_centroids;
#if MAX_ITER
    iterPrevCentroids = (int*)_prevCentroids;
#endif
    iterClusterSizes = _clusterSizes;
#if MAX_ITER
    centroidsDist = 0;
#endif
    for (int i=0; i<_nColors; ++i)
    {
      if (*iterClusterSizes)
      {
        *(iterCentroids)   /= *iterClusterSizes;
        *(iterCentroids+1) /= *iterClusterSizes;
        *(iterCentroids+2) /= *iterClusterSizes;

#if MAX_ITER
        centroidsDist += DIST((*(iterPrevCentroids)   - *(iterCentroids)));
        centroidsDist += DIST((*(iterPrevCentroids+1) - *(iterCentroids+1)));
        centroidsDist += DIST((*(iterPrevCentroids+2) - *(iterCentroids+2)));
#endif
      }
      ++iterClusterSizes;
#if MAX_ITER
      iterPrevCentroids+=4;
#endif
      iterCentroids+=4;
    }

#if MAX_ITER
    centroidsDist /= _nColors;

    //    std::cout << centroidsDist << std::endl;

    if (centroidsDist < CENTROIDS_THRESHOLD)
      break;
#endif
  }

  // remap colors
  iterClusters = _clusters;
  for (int y=0;y<_iterSizeY;y++)
  {
    _imageOut = (unsigned char*)&_outData[y*_iterSizeX];
    for (int x=0;x<_iterSizeX;x++)
    {
      iterCentroids  = (int*)&_centroids[*iterClusters];
      *(_imageOut)   = *(iterCentroids);   // r
      *(_imageOut+1) = *(iterCentroids+1); // g
      *(_imageOut+2) = *(iterCentroids+2); // b
      *(_imageOut+3) = *(iterCentroids+3); // a

      _imageOut+=4;
      ++iterClusters;
    }
  }

}
