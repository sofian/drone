#include "Gear_ClusteredDither.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ClusteredDither, Gear_ClusteredDither, "ClusteredDither")

Gear_ClusteredDither::Gear_ClusteredDither(Engine *engine, std::string name)
  : Gear(engine, "ClusteredDither", name), _threshold(0), _order(0)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _AMOUNT_IN = addPlugSignalIn("ClusterIN", 16);
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

bool Gear_ClusteredDither::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

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
    maxClusterSizeY = _clusterSize - ( y % _clusterSize);
    maxClusterSizeY = MIN( maxClusterSizeY, _sizeY-y );

    for (int x=0; x<_sizeX; x+=_clusterSize - (x % _clusterSize))
    {
      maxClusterSizeX = _clusterSize - ( x % _clusterSize);
      maxClusterSizeX = MIN( maxClusterSizeX, _sizeX-x );

      for (int yCell=0; yCell<maxClusterSizeY; ++yCell)
      {
        // *** OPTIM : y'aurait  moyen de faire ca un peu mieux...
        iterData    = (unsigned char*)&_data[(y+yCell)*_sizeX + x];
        iterOutData = (unsigned char*)&_outData[(y+yCell)*_sizeX + x];
        
        for (int xCell=0; xCell<maxClusterSizeX; ++xCell)
        {
          
          int ry = (y + yCell) * 3;
          int rx = (x + xCell) * 3;

			    /* Make sure rx and ry are positive and within
			     * the range 0 .. width-1 (incl).  Can't use %
			     * operator, since its definition on negative
			     * numbers is not helpful.  Can't use ABS(),
			     * since that would cause reflection about the
			     * x- and y-axes.  Relies on integer division
			     * rounding towards zero. */
          // *** OPTIM ici...
			    rx -= ((rx - ISNEG(rx)*(_width-1)) / _width) * _width;
			    ry -= ((ry - ISNEG(ry)*(_width-1)) / _width) * _width;

          // Grayscale.
          int total = *iterData++;
          total += *iterData;
          total += *iterData++;
          total += *iterData++;
          iterData++;
          total >>=2;

          // *** OPTIM : unroll loop
          int sum = 0;

          for(int sy=-1; sy<=1; sy++)
          {
				    for(int sx=-1; sx<=1; sx++)
				    {
              int ty = ry + sy;
              int tx = rx + sx;
             
//               if (tx < 0) tx += _width;
//               else if (tx >= _width) tx -= _width;
//               if (ty < 0) ty += _width;
//               else if (ty >= _width) ty -= _width;
              
              // *** OPTIM : precalculer les BARTLETT (en fait on se debarasse de cette macro tout simplement)
              if (total > _threshold[ty*_width + tx])
                sum += BARTLETT[sx+1][sy+1];
            }
          }

          sum >>= 4;
          
          *iterOutData++ = sum;
          *iterOutData++ = sum;
          *iterOutData++ = sum;
          *iterOutData++;

        }

      }
      
    }
  }

}

