#include "Gear_ClusteredDither.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ClusteredDither, Gear_ClusteredDither, "ClusteredDither")

Gear_ClusteredDither::Gear_ClusteredDither(Engine *engine, std::string name)
  : Gear(engine, "ClusteredDither", name), _threshold(0), _order(0),_clusterSize(0),  _spotType(ROUND)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _AMOUNT_IN_A = addPlugSignalIn("ClusterIN", 16);
  _AMOUNT_IN_B = addPlugSignalIn("SpotIN", ROUND);
}

Gear_ClusteredDither::~Gear_ClusteredDither()
{
  free(_threshold);
  free(_order);
}

void Gear_ClusteredDither::init()
{
  _clusterSize = (int)_AMOUNT_IN_A->buffer()[0];
  _spotType = (eSpotType)CLAMP((int)_AMOUNT_IN_B->buffer()[0],SQUARE,LINE);
  _width = _clusterSize * 3;
  computeThreshold();
}

void Gear_ClusteredDither::onUpdateSettings()
{
}

bool Gear_ClusteredDither::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

#define ISNEG(x)	(((x) < 0)? 1 : 0)

void Gear_ClusteredDither::runVideo()
{
  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());

  _sizeX = _image->sizeX();
  _sizeY = _image->sizeY();
  
  _data = _image->_data;    
  _outData = _outImage->_data;
  
  unsigned char *iterData = (unsigned char*)_data;
  unsigned char *iterOutData = (unsigned char*)_outData;

  int maxClusterSizeX;
  int maxClusterSizeY;
  int minClusterSizeX;
  int minClusterSizeY;
   
  // If cluster size has changed, recompute threshold matrix.
  if (_clusterSize != (int)_AMOUNT_IN_A->buffer()[0])
  {
    _clusterSize = (int)_AMOUNT_IN_A->buffer()[0];
    _width = _clusterSize * 3;
    computeThreshold();
  }

  // Set spot type.
  eSpotType tmpSpotType = (eSpotType)CLAMP((int)_AMOUNT_IN_B->buffer()[0],SQUARE,LINE);
  if (tmpSpotType != _spotType)
  {
    _spotType = tmpSpotType;
    computeThreshold();
  }
  
  int yStart = ((_sizeY/2-_clusterSize/2) % _clusterSize);
  if (yStart>=0)
    yStart -= _clusterSize;
  int xStart = ((_sizeX/2-_clusterSize/2) % _clusterSize) - _clusterSize;
  if (xStart>=0)
    xStart -= _clusterSize;

  // *** OPTIM : les modulo et les MIN...
  for (int y=yStart; y<_sizeY; y += _clusterSize)
  {
    minClusterSizeY = -MIN(0, y);
    maxClusterSizeY = MIN( _clusterSize, _sizeY-y );

    for (int x=xStart; x<_sizeX; x+=_clusterSize)
    {
      //      std::cout << x << "," << y << std::endl;
      minClusterSizeX = -MIN(0, x);
      maxClusterSizeX = MIN( _clusterSize, _sizeX-x );
      
      for (int yCell=minClusterSizeY; yCell<maxClusterSizeY; ++yCell)
      {
        // *** OPTIM : y'aurait  moyen de faire ca un peu mieux...
        iterData    = (unsigned char*)&_data[(y+yCell)*_sizeX + x];
        iterOutData = (unsigned char*)&_outData[(y+yCell)*_sizeX + x];

        /* Make sure rx and ry are positive and within
         * the range 0 .. width-1 (incl).  Can't use %
         * operator, since its definition on negative
         * numbers is not helpful.  Can't use ABS(),
         * since that would cause reflection about the
         * x- and y-axes.  Relies on integer division
         * rounding towards zero. */

        int ry = (y + yCell) * 3;
        ry -= ((ry - ISNEG(ry)*(_width-1)) / _width) * _width;
        
        for (int xCell=minClusterSizeX; xCell<maxClusterSizeX; ++xCell)
        {
          int rx = (x + xCell) * 3;
          rx -= ((rx - ISNEG(rx)*(_width-1)) / _width) * _width;
          
          // Compute and copy value.
          memset(iterOutData, getValue(intensity(iterData), rx, ry), 4*sizeof(unsigned char));
          iterData+=4;
          iterOutData+=4;
        }

      }
      
    }
  }

}

//#undef ISNEG
