#include "frei0r.hpp"

#include <algorithm>
#include <cmath>

class lissajous0r: public frei0r::source
{
public:
  lissajous0r(unsigned int width, unsigned int height)
  {
    register_param(r_x,"ratiox","x-ratio");
    register_param(r_y,"ratioy","y-ratio");
  }

  
  virtual void update()
  {
    std::fill(out, out+width*height, 0x00000000);

    double rx=1.0/(0.999999-r_x);
    double ry=1.0/(0.999999-r_y);
    
    
    const unsigned int samples=100000;

    for (unsigned int i=0;i != samples; ++i)
      {
	double t = 6.28*(double)i/(double)samples;
	unsigned int x = static_cast<unsigned int>((double)width*(1.0+sin(t*rx))/2.0);
 	unsigned int y = static_cast<unsigned int>((double)height*(1.0+cos(t*ry))/2.0);

	out[(width*y)+x]=0xffffffff;
	
      }
  }
private:
  double r_x;
  double r_y;
};


frei0r::construct<lissajous0r> plugin("Lissajous0r",
				   "Generates Lissajous0r images",
				   "Martin Bayer",
				   0,1);

