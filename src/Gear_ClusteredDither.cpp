#include "Gear_ClusteredDither.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ClusteredDither, Gear_ClusteredDither, "ClusteredDither")

Gear_ClusteredDither::Gear_ClusteredDither(Engine *engine, std::string name)
  : Gear(engine, "ClusteredDither", name)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _AMOUNT_IN = addPlugSignalIn("ClusterIN", 16);
}

Gear_ClusteredDither::~Gear_ClusteredDither()
{
}

bool Gear_ClusteredDither::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

#define SPOT_ROUND(x, y) 1-x*x-y*y
#define BOUNDS(a,x,y)	((a < x) ? x : ((a > y) ? y : a))

void Gear_ClusteredDither::runVideo()
{
  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());

  _clusterSize = (int)_AMOUNT_IN->buffer()[0];
  
  _sizeX = _image->sizeX();
  _sizeY = _image->sizeY();
  
  _data = _image->_data;    
  _outData = _outImage->_data;

  int clusterSize2 = _clusterSize*_clusterSize;
  
  unsigned char *iterData = (unsigned char*)_data;
  unsigned char *iterOutData = (unsigned char*)_outData;

  unsigned char *threshold = (unsigned char*)malloc(clusterSize2*sizeof(unsigned char));
  order_t *order = (order_t*)malloc(clusterSize2*sizeof(order_t));

  int maxClusterSizeY;
  int maxClusterSizeX;
  
  for (int y=0; y<_sizeY; y += _clusterSize)
  {
    if (y + _clusterSize > _sizeY)
      maxClusterSizeY = _sizeY - y;
    else
      maxClusterSizeY = _clusterSize;
    
    for (int x=0; x<_sizeX; x+=_clusterSize)
    {

      if (x + _clusterSize > _sizeX)
        maxClusterSizeX = _sizeX - x;
      else
        maxClusterSizeX = _clusterSize;

      int i=0;
      // inside cell
      for (int yCell=0; yCell<maxClusterSizeY; ++yCell)
      {
        double sy = 2*(double)yCell / (_clusterSize-1) - 1;
        for (int xCell=0; xCell<maxClusterSizeX; ++xCell)
        {
          double sx = 2*(double)xCell / (_clusterSize-1) - 1;
          double val = SPOT_ROUND(sx, sy);
          val = BOUNDS(val, -1, 1);
          
          order[i].index = i;
          order[i].value = val;
          ++i; 
        }
      }

      /* now sort array of (point, value) pairs in ascending order */
      qsort(order, clusterSize2, sizeof(order_t), order_cmp);

      for(i=0; i < clusterSize2; i++)
      {
        /* thresh[] contains values from 0 .. 254.  The reason for not
         * including 255 is so that an image value of 255 remains
         * unmolested.  It would be bad to filter a completely white
         * image and end up with black speckles.  */
        //         if (balanced)
        //        threshold[order[i].index] = order[i].value * 0xfe; // *** changer seulement si on utilise des DIAMONDS
//         else
        threshold[order[i].index] = i * 0xff / clusterSize2; // *** pas besoin de .value dans c'cas là...
      }

      for (int yCell=0; yCell<maxClusterSizeY; ++yCell)
      {

        iterData    = (unsigned char*)&_data[(y+yCell)*_sizeX + x];
        iterOutData = (unsigned char*)&_outData[(y+yCell)*_sizeX + x];
        
        for (int xCell=0; xCell<maxClusterSizeX; ++xCell)
        {
          // Grayscale.
          int total = *iterData++;
          total += *iterData;
          total += *iterData++;
          total += *iterData++;
          iterData++;
          total >>=2;

          if (total > threshold[yCell*maxClusterSizeX + xCell])
            // color black
            memset(iterOutData, 255, 4*sizeof(unsigned char));
          else
            memset(iterOutData, 0, 4*sizeof(unsigned char));

          iterOutData += 4;

        }

      }
      
    }
  }

  free(threshold);
  free(order);
}

