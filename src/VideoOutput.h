#ifndef VIDEOOUTPUT_INCLUDED
#define VIDEOOUTPUT_INCLUDED

#include "ColorSpace.h"
#include "MatrixType.h"

class VideoOutput
{
public:
  VideoOutput() : _xRes(0), _yRes(0) {}
  virtual ~VideoOutput(){}

  virtual bool init(int xRes, int yRes, bool fullscreen)=0;    
  virtual void render(const MatrixType<RGBA> &image)=0;
  virtual void fullscreen(bool fs)=0;
  virtual void destroy()=0;

protected:
  int _xRes, _yRes;
  int _bpp;

};


#endif

