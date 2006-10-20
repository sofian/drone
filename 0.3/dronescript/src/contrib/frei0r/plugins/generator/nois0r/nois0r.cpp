#include "frei0r.hpp"

#include <algorithm>

struct wnoise
{
  wnoise(unsigned int s)
    : seed(s) {}
  
  unsigned int seed;

  unsigned int operator()()
  {
    seed *= 3039177861U; // parameter for LCG
    unsigned char rd = seed >> 24;
    return (rd | rd << 8 | rd << 16);
  }


};

class nois0r : public frei0r::source
{
public:
  nois0r(unsigned int width, unsigned int height)
  {
  }

  
  virtual void update()
  {
    wnoise wn(0x0f0f0f0f ^ (unsigned int)(time*100000.0));
    
    std::generate(out, out+width*height, wn);
  }
};


frei0r::construct<nois0r> plugin("Nois0r",
				   "Generates white noise images",
				   "Martin Bayer",
				   0,2);

