#ifndef VIDEOOUTPUTSHM_INCLUDED
#define VIDEOOUTPUTSHM_INCLUDED

#include "VideoOutputX11Base.h"

namespace X11
{

class Canvas;

class VideoOutputShm : public VideoOutputX11Base
{
public:
  VideoOutputShm();
  ~VideoOutputShm();

  void fullscreen(bool fs);
  void destroy();
  bool init(int xRes, int yRes, bool fullscreen);
  void render(MatrixType<RGBA> &image);

protected:


  XImage* createXImage(int sizeX, int sizeY);
  void destroyXImage();
  void onResize(int sizeX, int sizeY);

private:


  XImage* _xImage;
  int _frameSizeX;
  int _frameSizeY;
  int _frameSize;

};
}
#endif
