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
#define BARTLETT(x,y)	((2-abs(x)) * (2-abs(y)))
#define BOUNDS(a,x,y)	((a < x) ? x : ((a > y) ? y : a))
#define ISNEG(x)	(((x) < 0)? 1 : 0)

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

  //int clusterSize2 = _clusterSize*_clusterSize;
  
  unsigned char *iterData = (unsigned char*)_data;
  unsigned char *iterOutData = (unsigned char*)_outData;

  int maxClusterSizeY;
  int maxClusterSizeX;

  int width = _clusterSize * 3;
  int width2 = width*width;

  unsigned char *threshold = (unsigned char*)malloc(width2*sizeof(unsigned char));
  order_t *order = (order_t*)malloc(width2*sizeof(order_t));

  /* Bartlett window matrix optimisation */
  int w002 = BARTLETT(0,0) * BARTLETT(0,0);

  for (int y=0; y<_sizeY; y += _clusterSize - (y % _clusterSize))
  {
    maxClusterSizeY = _clusterSize - ( y % _clusterSize);
    maxClusterSizeY = MIN( maxClusterSizeY, _sizeY-y );

    for (int x=0; x<_sizeX; x+=_clusterSize - (x % _clusterSize))
    {
      maxClusterSizeX = _clusterSize - ( x % _clusterSize);
      maxClusterSizeX = MIN( maxClusterSizeX, _sizeX-x );

      int i=0;
      // inside cell
      for (int yCell=0; yCell<width; ++yCell)
      {
        double sy = 2*(double)yCell / (width-1) - 1;
        for (int xCell=0; xCell<width; ++xCell)
        {
          double sx = 2*(double)xCell / (width-1) - 1;
          double val = SPOT_ROUND(sx, sy);
          val = BOUNDS(val, -1, 1);
          
          order[i].index = i;
          order[i].value = val;
          ++i; 
        }
      }

      /* now sort array of (point, value) pairs in ascending order */
      //      qsort(order, width2, sizeof(order_t), order_cmp);

      for (i=0; i < width2; i++)
      {
        /* thresh[] contains values from 0 .. 254.  The reason for not
         * including 255 is so that an image value of 255 remains
         * unmolested.  It would be bad to filter a completely white
         * image and end up with black speckles.  */
        threshold[order[i].index] = i * 0xff / width2; // *** pas besoin de .value dans c'cas là...
      }

      for (int yCell=0; yCell<maxClusterSizeY; ++yCell)
      {
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
			    rx -= ((rx - ISNEG(rx)*(width-1)) / width) * width;
			    ry -= ((ry - ISNEG(ry)*(width-1)) / width) * width;

          // Grayscale.
          int total = *iterData++;
          total += *iterData;
          total += *iterData++;
          total += *iterData++;
          iterData++;
          total >>=2;

          int sum = 0;
          
          for(int sy=-1; sy<=1; sy++)
          {
				    for(int sx=-1; sx<=1; sx++)
				    {
              int ty = ry + sy;
              int tx = rx + sx;
              
              while (tx < 0)  tx += width;
              while (ty < 0)  ty += width;
              while (tx >= width)  tx -= width;
              while (ty >= width)  ty -= width;
              
              if (total > threshold[ty*width + tx])
                sum += 0xff * BARTLETT(sx, sy);
            }
          }

          sum /= w002;
          
          *iterOutData++ = sum;
          *iterOutData++ = sum;
          *iterOutData++ = sum;
          *iterOutData++;

        }

      }
      
    }
  }

  free(threshold);
  free(order);
}

