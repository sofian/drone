#include "frei0r.hpp"

#include <algorithm>

class onecol0r : public frei0r::source
{
public:
  onecol0r(unsigned int width, unsigned int height)
  {
    register_param(color,"Color","the color of the image");
  }
  
  virtual void update()
  {
    unsigned int col;
    unsigned char* c = reinterpret_cast<unsigned char*>(&col);

    c[0]=static_cast<unsigned char>(color.b*255);
    c[1]=static_cast<unsigned char>(color.g*255);
    c[2]=static_cast<unsigned char>(color.r*255);
    c[3]=0;
    
    std::fill(out, out+width*height, col);
  }
  
private:
  f0r_param_color color;
};


frei0r::construct<onecol0r> plugin("onecol0r",
				   "image with just one color",
				   "Martin Bayer",
				   0,1);

