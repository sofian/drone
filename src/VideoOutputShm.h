#ifndef VIDEOOUTPUTSHM_INCLUDED
#define VIDEOOUTPUTSHM_INCLUDED

#include "VideoOutputX11Base.h"

class Canvas;

class VideoOutputShm : public VideoOutputX11Base
{
public:
  VideoOutputShm();
  ~VideoOutputShm();

  void fullscreen(bool fs);
  void destroy();
  bool init(int xRes, int yRes, bool fullscreen);
  void render(Canvas &canvas);

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

#endif
