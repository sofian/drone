#include "Gear_ClusteredDither.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ClusteredDither, Gear_ClusteredDither, "ClusteredDither")
const std::string Gear_ClusteredDither::SETTING_SPOT_FUNCTION = "SPOT_FUNCTION (0=square, 1=diamond, 2=round, 3=line)";

Gear_ClusteredDither::Gear_ClusteredDither(Engine *engine, std::string name)
  : Gear(engine, "ClusteredDither", name), _threshold(0), _order(0), _spotType(ROUND)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _AMOUNT_IN = addPlugSignalIn("ClusterIN", 16);

  _settings.add(Property::INT, SETTING_SPOT_FUNCTION)->valueInt((int)ROUND);
}

Gear_ClusteredDither::~Gear_ClusteredDither()
{
  free(_threshold);
  free(_order);
}

void Gear_ClusteredDither::init()
{
  _clusterSize = (int)_AMOUNT_IN->buffer()[0];
  _width = _clusterSize * 3;
  computeThreshold();
}

void Gear_ClusteredDither::onUpdateSettings()
{
  _spotType = (eSpotType)_settings.get(SETTING_SPOT_FUNCTION)->valueInt();
  computeThreshold();
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

  int maxClusterSizeY;
  int maxClusterSizeX;

  // If cluster size has changed, recompute threshold matrix.
  if (_clusterSize != (int)_AMOUNT_IN->buffer()[0])
  {
    _clusterSize = (int)_AMOUNT_IN->buffer()[0];
    _width = _clusterSize * 3;
    computeThreshold();
  }

  // *** OPTIM : les modulo et les MIN...
  for (int y=0; y<_sizeY; y += _clusterSize - (y % _clusterSize))
  {
    maxClusterSizeY = MIN( _clusterSize, _sizeY-y );

    for (int x=0; x<_sizeX; x+=_clusterSize - (x % _clusterSize))
    {
      maxClusterSizeX = MIN( _clusterSize, _sizeX-x );

      for (int yCell=0; yCell<maxClusterSizeY; ++yCell)
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
        
        for (int xCell=0; xCell<maxClusterSizeX; ++xCell)
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
