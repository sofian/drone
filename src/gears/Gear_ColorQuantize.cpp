#include "Gear_ColorQuantize.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ColorQuantize, Gear_ColorQuantize, "ColorQuantize")

Gear_ColorQuantize::Gear_ColorQuantize(Engine *engine, std::string name)
: Gear(engine, "ColorQuantize", name), _nColors(DEFAULT_N_COLORS), _clusters(0)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");

  _centroids = new RGBAint[_nColors];

  // create initial centroids randomly
  srand(2732791);
  for (int i=0;i<_nColors;++i)
  {
    _centroids[i].r = rand()%256;
    _centroids[i].g = rand()%256;
    _centroids[i].b = rand()%256;
    _centroids[i].a = 0;
  }

  _clusterSizes = (int*)malloc(_nColors*sizeof(int));
}

Gear_ColorQuantize::~Gear_ColorQuantize()
{
  delete[] _centroids;
  free(_clusters);
  free(_clusterSizes);
}

bool Gear_ColorQuantize::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_ColorQuantize::runVideo()
{
  // initialize

  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());

  _iterSizeX = _image->sizeX();
  _iterSizeY = _image->sizeY();
  _iterSize = _iterSizeX * _iterSizeY;

  _clusters = (int*) realloc(_clusters, _iterSize * sizeof(int));

  _data = _image->_data;
  _outData = _outImage->_data;

  int *iterClusters;
  int *iterCentroids;
  int *iterClusterSizes;

  int minDist;
  int diff;
  int dist;

  memset(_clusterSizes, 0, _nColors*sizeof(int));

  // find cluster ownership of each pixel
  iterClusters = _clusters;
  _imageIn = (unsigned char*)_data;

  for (int p=0; p<_iterSize; ++p)
  {
    minDist = INT_MAX; // very big number
    iterCentroids = (int*)_centroids;

    // get the RGB value of the pixel
    _r = (int)*_imageIn++;
    _g = (int)*_imageIn++;
    _b = (int)*_imageIn;
    _imageIn+=2;

    // find minimum distance
    for (int i=0; i<_nColors; ++i)
    {
      // euclidian distance
      diff = _r - *iterCentroids++; // r diff
      dist = diff*diff;
      diff = _g - *iterCentroids++; // g diff
      dist += diff*diff;
      diff = _b - *iterCentroids;   // b diff
      dist += diff*diff;

      iterCentroids+=2;

      if (dist < minDist)
      {
        *iterClusters = i;
        minDist = dist;
      }
    }

    _clusterSizes[*iterClusters]++;
    ++iterClusters;
  }

  // calculate new centroids

  // initialize to zero
  iterCentroids = (int*)_centroids;
  iterClusterSizes = _clusterSizes;
  for (int i=0; i<_nColors; ++i)
  {
    if (*iterClusterSizes > 0)
      memset(iterCentroids, 0, sizeof(RGBAint));

    ++iterClusterSizes;
  }

  // accumulate colors
  iterClusters = _clusters;
  _imageIn = (unsigned char*)_data;
  for (int p=0; p<_iterSize; ++p)
  {
    iterCentroids = (int*)&_centroids[*iterClusters];
    *iterCentroids++ += (int)*_imageIn++; // r
    *iterCentroids++ += (int)*_imageIn++; // g
    *iterCentroids   += (int)*_imageIn;   // b

    _imageIn+=2;
    ++iterClusters;
  }

  // take the mean
  iterCentroids = (int*)_centroids;
  iterClusterSizes = _clusterSizes;
  for (int i=0; i<_nColors; ++i)
  {
    if (*iterClusterSizes)
    {
      *iterCentroids++ /= *iterClusterSizes;
      *iterCentroids++ /= *iterClusterSizes;
      *iterCentroids   /= *iterClusterSizes;

      iterCentroids+=2;
    } else
      iterCentroids+=4;

    ++iterClusterSizes;
  }

  // remap colors
  iterClusters = _clusters;
  _imageOut = (unsigned char*)_outData;
  for (int p=0; p<_iterSize; ++p)
  {
    iterCentroids  = (int*)&_centroids[*iterClusters];
    *_imageOut++ = (unsigned char)*iterCentroids++; // r
    *_imageOut++ = (unsigned char)*iterCentroids++; // g
    *_imageOut++ = (unsigned char)*iterCentroids++; // b
    *_imageOut++ = (unsigned char)*iterCentroids;   // a

    ++iterClusters;
  }
}
