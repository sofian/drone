#include "frei0r.hpp"

/* Clamps a int32-range int between 0 and 255 inclusive. */
unsigned char CLAMP0255(int32_t a)
{
  return (unsigned char)
    ( (((-a) >> 31) & a)  // 0 if the number was negative
      | (255 - a) >> 31); // -1 if the number was greater than 255
}

class sobel : public frei0r::filter
{
public:
  sobel(unsigned int width, unsigned int height)
  {
  }
  
  virtual void update()
  {
    std::copy(in, in + width*height, out);
    for (unsigned int y=1; y<height-1; ++y)
    {
      for (unsigned int x=1; x<width-1; ++x)
      {
        // XXX faut eviter le * width en y allant par rangee
        unsigned char *p1 = (unsigned char *)&in[(y-1)*width+(x-1)];
        unsigned char *p2 = (unsigned char *)&in[(y-1)*width+x];
        unsigned char *p3 = (unsigned char *)&in[(y-1)*width+(x+1)];
        unsigned char *p4 = (unsigned char *)&in[y*width+(x-1)];
        unsigned char *p6 = (unsigned char *)&in[y*width+(x+1)];
        unsigned char *p7 = (unsigned char *)&in[(y+1)*width+(x-1)];
        unsigned char *p8 = (unsigned char *)&in[(y+1)*width+x];
        unsigned char *p9 = (unsigned char *)&in[(y+1)*width+(x+1)];
        
        unsigned char *g = (unsigned char *)&out[y*width+x];
        
        for (int i=0; i<3; ++i)
          g[i] = CLAMP0255(
                           abs(p1[i] + p2[i]*2 + p3[i] - p7[i] - p8[i]*2 - p9[i]) +
                           abs(p3[i] + p6[i]*2 + p9[i] - p1[i] - p4[i]*2 - p7[i]) );
        
      }
    }
  }
};


frei0r::construct<sobel> plugin("Sobel",
				     "Sobel filter",
				     "Drone",
				     0,1);

